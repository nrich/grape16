#include <cstdlib>
#include <memory>
#include <functional>
#include <map>

#include <iostream>

#include <ctime>
#include <ratio>
#include <chrono>

#if __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

#ifndef __EMSCRIPTEN__
#include "ezOptionParser.hpp"
#endif

#ifdef _WIN32
#include "mingw.thread.h"
#else
#include <thread>
#endif

#include "Client/State.h"

#ifdef __EMSCRIPTEN__
#include <gl4esinit.h>
#include "Renderer/Immediate.h"
#else
#include "Renderer/Immediate.h"
#endif

#include "Common/DisplayMode.h"

#if MINBUILD
#else
#include "Sys/SDL2.h"
#endif

#ifndef __EMSCRIPTEN__
#include "Sys/GLFW.h"
#endif

#ifndef _WIN32
#ifndef __EMSCRIPTEN__
#include "Sys/NCurses.h"
#include "Renderer/NCurses.h"
#include "Sys/SFML.h"
#endif
#endif

#include "Emulator/VM.h"
#include "Emulator/Basic.h"
#include "Emulator/Assembler.h"

#include "Client/DebugState.h"
#include "Client/DisplayMenuState.h"
#include "Client/EmulatorState.h"

#define CLOCK_8MHz_at_60FPS   133333
#define CLOCK_16MHz_at_60FPS  266667
#define CLOCK_33MHz_at_60FPS  550000
#define CLOCK_66MHz_at_60FPS  1100000
#define CLOCK_100MHz_at_60FPS  1666667
#define CLOCK_133MHz_at_60FPS  2216667
#define CLOCK_166MHz_at_60FPS  2766667
#define CLOCK_200MHz_at_60FPS  3333333

#ifdef SYS32
    #define EXEHEADER "GR32"
    #define APPNAME "Grape32"
#else
    #define EXEHEADER "GR16"
    #define APPNAME "Grape16"
#endif

std::shared_ptr<Emulator::Program> loadBinary(const std::string &input) {
    std::ifstream infile(input, std::ios_base::binary);

    if (!infile.is_open()) {
        std::cerr << "Could not open `" << input << "'" << std::endl;
        exit(-1);
    }

    std::vector<uint8_t> buffer((std::istreambuf_iterator<char>(infile)), (std::istreambuf_iterator<char>()));

    std::string header(buffer.begin(), buffer.begin()+4);

    if (header != EXEHEADER) {
        std::cerr << "Invalid header `" << header << "'" << std::endl;
        exit(-1);
    }

    Emulator::Program program({buffer.begin()+4, buffer.end()});

    return std::make_shared<Emulator::Program>(program);
}

std::shared_ptr<Emulator::Program> loadBasic(const std::string &input, bool debug) {
    Emulator::Program program;
    auto lines = parseFile(input);

    if (debug) {
        for (auto line : lines) {
            std::cerr << line.first;

            for (auto token : line.second) {
                std::cerr << " " << (int)token.type << ":" << token.str << ":" << (int)token.lbp;
            }
            std::cerr << std::endl;
        }
    }

    compile(lines, program);

    return std::make_shared<Emulator::Program>(program);
}

std::shared_ptr<Emulator::Program> loadAssembly(const std::string &input, bool debug) {
    Emulator::Program program;
    auto lines = parseAsmFile(input);

    assemble(lines, program);

    return std::make_shared<Emulator::Program>(program);
}

#if __EMSCRIPTEN__
void emscripten_loop(void *userdata) {
    auto *args = static_cast<std::pair<std::shared_ptr<Sys::Base>, std::shared_ptr<Client::State>> *>(userdata);

    auto sys = args->first;
    auto clientState = args->second;

    static uint32_t lastRender = sys->getTicks();
    uint32_t renderTime = sys->getTicks();

    while (sys->handleEvents(clientState)) {
        lastRender = renderTime;

        auto t1 = std::chrono::high_resolution_clock::now();

        sys->clearScreen();
        renderTime = sys->getTicks();
        clientState->tick(renderTime - lastRender);
        clientState->render(renderTime - lastRender);

        sys->swapBuffers();

        auto t4 = std::chrono::high_resolution_clock::now();
        auto taken = std::chrono::duration_cast<std::chrono::milliseconds>(t4 - t1).count();

        if (std::chrono::milliseconds(16 - taken).count() > 0)
            emscripten_sleep(std::chrono::milliseconds(16 - taken).count());
    }
}
#endif

int main(int argc, char **argv) {
#ifndef __EMSCRIPTEN__
    ez::ezOptionParser opt;

    opt.overview = "grape16 micro emulator";
    opt.syntax = std::string(argv[0]) + " [OPTIONS] [runfile]\n";
    opt.example = std::string(argv[0]) + " -b test.bas\n";
    opt.footer = std::string(argv[0]) + " v" + std::string(VERSION) + "\n";

    opt.add(
        "", // Default.
        0, // Required?
        0, // Number of args expected.
        0, // Delimiter if expecting multiple args.
        "Display usage instructions.", // Help description.
        "-h",     // Flag token.
        "-help",  // Flag token.
        "--help", // Flag token.
        "--usage" // Flag token.
    );

    opt.add(
        "", // Default.
        0, // Required?
        0, // Number of args expected.
        0, // Delimiter if expecting multiple args.
        "Run in debug mode", // Help description.
        "-d",     // Flag token.
        "-dbg",   // Flag token.
        "--debug" // Flag token.
    );

    opt.add(
        "", // Default.
        0, // Required?
        0, // Number of args expected.
        0, // Delimiter if expecting multiple args.
        "Step through ops", // Help description.
        "-s",     // Flag token.
        "-stp",   // Flag token.
        "--step"  // Flag token.
    );

    opt.add(
        "", // Default.
        0, // Required?
        0, // Number of args expected.
        0, // Delimiter if expecting multiple args.
        "BASIC file input", // Help description.
        "-b",     // Flag token.
        "-basic",   // Flag token.
        "--basic"  // Flag token.
    );

    opt.add(
        "", // Default.
        0, // Required?
        0, // Number of args expected.
        0, // Delimiter if expecting multiple args.
        "Assembly file input", // Help description.
        "-a",     // Flag token.
        "-asm",   // Flag token.
        "--asm"  // Flag token.
    );

    opt.add(
        "", // Default.
        0, // Required?
        0, // Number of args expected.
        0, // Delimiter if expecting multiple args.
        "Disassemble", // Help description.
        "-p"     // Flag token.
    );

    opt.add(
        "-", // Default.
        0, // Required?
        1, // Number of args expected.
        0, // Delimiter if expecting multiple args.
        "Disassemble output", // Help description.
        "-o"     // Flag token.
    );

    opt.add(
        "", // Default.
        0, // Required?
        0, // Number of args expected.
        0, // Delimiter if expecting multiple args.
        "optimize output", // Help description.
        "-O",     // Flag token.
        "--optimize"  // Flag token.
    );


    opt.add(
#ifdef SYS32
        "7", // Default.
#else
        "2", // Default.
#endif
        0, // Required?
        1, // Number of args expected.
        0, // Delimiter if expecting multiple args.
        "CPU speed (0=8MHz, 1=16MHz, 2=33MHz, 3=66MHz, 4=100MHz, 5=133MHz, 6=166Mhz, 7=200MHz)", // Help description.
        "-t",     // Flag token.
        "-turbo",   // Flag token.
        "--turbo"  // Flag token.
    );

    opt.add(
#ifdef SYS32
        "7", // Default.
#else
        "2", // Default.
#endif
        0, // Required?
        1, // Number of args expected.
        0, // Delimiter if expecting multiple args.
#ifdef SYS32
        "Memory size (0=1MB, 1=2MB, 2=4MB, 3=8MB, 4=16MB, 5=32MB, 6=64MB, 7=128MB)", // Help description.
#else
        "Memory size (0=1MB, 1=2MB, 2=4MB, 3=8MB)", // Help description.
#endif
        "-m",     // Flag token.
        "-mem",   // Flag token.
        "--mem"  // Flag token.
    );

    opt.add(
        "", // Default.
        0, // Required?
        0, // Number of args expected.
        0, // Delimiter if expecting multiple args.
        "Refresh rate of 30Hz", // Help description.
        "-r",     // Flag token.
        "-refresh",   // Flag token.
        "--refresh"  // Flag token.
    );

    opt.add(
#ifdef _WIN32
#if MINBUILD
        "glfw", // Default.
#else
        "sdl2", // Default.
#endif
#else
        "ncurses", // Default.
#endif
        0, // Required?
        1, // Number of args expected.
        0, // Delimiter if expecting multiple args.
        "Display usage instructions.", // Help description.
        "-s",     // Flag token.
        "-sys",  // Flag token.
        "--sys" // Flag token.
    );

    opt.parse(argc, (const char**)argv);

    if (opt.isSet("-h")) {
        std::string usage;
        opt.getUsage(usage);
        std::cout << usage << std::endl;
        exit(1);
    }
#endif

    std::shared_ptr<Emulator::Program> program;

#ifndef __EMSCRIPTEN__
    if (opt.lastArgs.size() == 1) {
        if (opt.isSet("-a")) {
            program = loadAssembly(*opt.lastArgs[0], opt.isSet("-d"));
        } else if (opt.isSet("-b")) {
            program = loadBasic(*opt.lastArgs[0], opt.isSet("-d"));
        } else {
            program = loadBinary(*opt.lastArgs[0]);
        }

        if (opt.isSet("-O")) {
            auto optimized = optimize(*program);
            program = std::make_shared<Emulator::Program>(optimized);
        }

        if (opt.isSet("-o")) {
            std::string filename;
            opt.get("-o")->getString(filename);

            if (filename == "-") {
                if (opt.isSet("-p")) {
                    std::cout << ProgramAsString(*program, true);
                } else {
                    std::cerr << "Cannot write binary to stdout" << std::endl;
                }
            } else {
                std::ofstream file;

                if (opt.isSet("-p")) {
                    file.open(filename);
                } else {
                    file.open(filename, std::ios_base::binary);
                }

                if (file.is_open()) {
                    if (opt.isSet("-p")) {
                        file << ProgramAsString(*program, true);
                    } else {
                        file.write(EXEHEADER, 4);
                        file.write((const char*)program->Code().data(), program->Code().size());
                    }
                    file.close();
                }
            }
            exit(0);
        }
    } else {
#else
    initialize_gl4es();
    if (1) {
#endif
        program = std::make_shared<Emulator::Program>();
        program->add(Emulator::OpCode::NOP);
        program->add(Emulator::OpCode::HALT);
    }

    std::shared_ptr<Renderer::Base> renderer;
    std::shared_ptr<Sys::Base> sys;

#ifndef __EMSCRIPTEN__
    std::string sysname;

    opt.get("-s")->getString(sysname);

    if (sysname == "glfw") {
        sys = std::make_shared<Sys::GLFW>(APPNAME);
        renderer = std::make_shared<Renderer::Immediate>(sys->currentDisplayMode(), Common::AspectRatio::_4x3, 2);
#if MINBUILD
#else
    } else if (sysname == "sdl2") {
        sys = std::make_shared<Sys::SDL2>(APPNAME);
        renderer = std::make_shared<Renderer::Immediate>(sys->currentDisplayMode());
#endif
#ifndef _WIN32
    } else if (sysname == "sfml") {
        sys = std::make_shared<Sys::SFML>(APPNAME);
        renderer = std::make_shared<Renderer::Immediate>(sys->currentDisplayMode());
    } else if (sysname == "ncurses") {
        initscr();

        auto window = std::shared_ptr<WINDOW>(
            newwin(24, 42, 0, 0),
            delwin
        );

        sys = std::make_shared<Sys::NCurses>(APPNAME, window);
        renderer = std::make_shared<Renderer::NCurses>(window);
#endif
    } else {
        std::cerr << "Unknown system" << std::endl;
        exit(0);
    }

    uint32_t clockspeed = CLOCK_33MHz_at_60FPS;
    int turbomode;
    opt.get("-t")->getInt(turbomode);

    switch(turbomode) {
        case 0:
            clockspeed = CLOCK_8MHz_at_60FPS;
            break;
        case 1:
            clockspeed = CLOCK_16MHz_at_60FPS;
            break;
        case 2:
            clockspeed = CLOCK_33MHz_at_60FPS;
            break;
        case 3:
            clockspeed = CLOCK_66MHz_at_60FPS;
            break;
        case 4:
            clockspeed = CLOCK_100MHz_at_60FPS;
            break;
        case 5:
            clockspeed = CLOCK_133MHz_at_60FPS;
            break;
        case 6:
            clockspeed = CLOCK_166MHz_at_60FPS;
            break;
        case 7:
            clockspeed = CLOCK_200MHz_at_60FPS;
            break;
        default:
            clockspeed = UINT32_MAX;
            break;
    }

    uint32_t memsize = 0x003FFFFF;
    int mem;
    opt.get("-m")->getInt(mem);

    switch(mem) {
        case 0:
            memsize = 0x000FFFFF;
            break;
        case 1:
            memsize = 0x001FFFFF;
            break;
        case 2:
            memsize = 0x003FFFFF;
            break;
        case 3:
            memsize = 0x007FFFFF;
            break;
#ifdef SYS32
        case 4:
            memsize = 0x00FFFFFF;
            break;
        case 5:
            memsize = 0x01FFFFFF;
            break;
        case 6:
            memsize = 0x03FFFFFF;
            break;
        case 7:
            memsize = 0x07FFFFFF;
            break;
#endif
    }


    bool debug = opt.isSet("-d");
#else

#ifdef SYS32
    uint32_t clockspeed = CLOCK_200MHz_at_60FPS;
    uint32_t memsize = 0x0FFFFFFF;
#else
    uint32_t clockspeed = CLOCK_33MHz_at_60FPS;
    uint32_t memsize = 0x003FFFFF;
#endif
    bool debug = false;
    sys = std::make_shared<Sys::SDL2>(APPNAME);
    renderer = std::make_shared<Renderer::Immediate>(sys->currentDisplayMode());
#endif

    sys->keyRepeat(true);

    auto vm = std::make_shared<Emulator::VM>(memsize);

    auto debugState = std::make_shared<Client::DebugState>(vm);
    auto emulatorState = std::make_shared<Client::EmulatorState>(vm, program, clockspeed, debug);
    auto displayMenuState = std::make_shared<Client::DisplayMenuState>();

    auto clientState = std::make_shared<Client::State>(
        std::dynamic_pointer_cast<Renderer::Base>(renderer),
        std::dynamic_pointer_cast<Sys::Base>(sys),
        std::pair<uint32_t, std::shared_ptr<Client::BaseState>>(0, std::dynamic_pointer_cast<Client::BaseState>(emulatorState))
    );

    clientState->addState(1, std::dynamic_pointer_cast<Client::BaseState>(displayMenuState));
    clientState->addState(2, std::dynamic_pointer_cast<Client::BaseState>(debugState));

#if __EMSCRIPTEN__
    auto args = std::pair<std::shared_ptr<Sys::Base>, std::shared_ptr<Client::State>>(sys, clientState);
    emscripten_set_main_loop_arg(emscripten_loop, (void *)&args, -1, 1);
#else
    static uint32_t lastRender = sys->getTicks();
    uint32_t renderTime = sys->getTicks();

    while (sys->handleEvents(clientState)) {
        lastRender = renderTime;

        auto t1 = std::chrono::high_resolution_clock::now();
        sys->clearScreen();
        renderTime = sys->getTicks();
        clientState->tick(renderTime - lastRender);
        clientState->render(renderTime - lastRender);

        sys->swapBuffers();

        auto t4 = std::chrono::high_resolution_clock::now();
        auto taken = std::chrono::duration_cast<std::chrono::milliseconds>(t4 - t1).count();

        if (std::chrono::milliseconds(16 - taken).count() > 0)
            std::this_thread::sleep_for(std::chrono::milliseconds(16 - taken));
    }
#endif

    exit(0);
}
