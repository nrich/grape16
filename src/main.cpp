#include <cstdlib>
#include <memory>
#include <functional>
#include <map>

#include <iostream>

#include <ctime>
#include <ratio>
#include <chrono>

#include "ezOptionParser.hpp"

#ifdef _WIN32
#include "mingw.thread.h"
#else
#include <thread>
#endif

#include "Client/State.h"

#include "Renderer/Immediate.h"

#include "Common/DisplayMode.h"

#if MINBUILD
#else
#include "Sys/SDL2.h"
#include "Sys/SFML.h"
#endif

#include "Sys/GLFW.h"

#ifndef _WIN32
#include "Sys/NCurses.h"
#include "Renderer/NCurses.h"
#endif

#include "Emulator/VM.h"
#include "Emulator/Basic.h"

#include "Client/DebugState.h"
#include "Client/DisplayMenuState.h"
#include "Client/EmulatorState.h"

#define CLOCK_33MHz_at_60FPS  550000
#define CLOCK_66MHz_at_60FPS  1100000

std::shared_ptr<Emulator::Program> loadAssembly(const std::string &input) {
    std::ifstream infile(input, std::ios_base::binary);

    if (!infile.is_open()) {
        std::cerr << "Could not open `" << input << "'" << std::endl;
        exit(-1);
    }

    std::vector<uint8_t> buffer((std::istreambuf_iterator<char>(infile)), (std::istreambuf_iterator<char>()));

    Emulator::Program program(buffer);

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

int main(int argc, char **argv) {
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
        "Turbo mode (66MHz)", // Help description.
        "-t",     // Flag token.
        "-turbo",   // Flag token.
        "--turbo"  // Flag token.
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
        "glfw", // Default.
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

    std::shared_ptr<Emulator::Program> program;

    if (opt.lastArgs.size() == 1) {
        if (opt.isSet("-b")) {
            program = loadBasic(*opt.lastArgs[0], opt.isSet("-d"));
        } else {
            program = loadAssembly(*opt.lastArgs[0]);
        }
    } else {
        program = std::make_shared<Emulator::Program>();
        program->add(Emulator::OpCode::NOP);
        program->add(Emulator::OpCode::HALT);
    }

    std::shared_ptr<Renderer::Base> renderer;
    std::shared_ptr<Sys::Base> sys;
    std::string sysname;
    opt.get("-s")->getString(sysname);

    if (sysname == "glfw") {
        sys = std::make_shared<Sys::GLFW>("Grape16");
        renderer = std::make_shared<Renderer::Immediate>(sys->currentDisplayMode());
#if MINBUILD
#else
    } else if (sysname == "sdl2") {
        sys = std::make_shared<Sys::SDL2>("Grape16");
        renderer = std::make_shared<Renderer::Immediate>(sys->currentDisplayMode());
    } else if (sysname == "sfml") {
        sys = std::make_shared<Sys::SFML>("Grape16");
        renderer = std::make_shared<Renderer::Immediate>(sys->currentDisplayMode());
#endif
#ifndef _WIN32
    } else if (sysname == "ncurses") {
        initscr();

        auto window = std::shared_ptr<WINDOW>(
            newwin(24, 42, 0, 0),
            delwin
        );

        sys = std::make_shared<Sys::NCurses>("Grape16", window);
        renderer = std::make_shared<Renderer::NCurses>(window);
#endif
    } else {
        std::cerr << "Unknown system" << std::endl;
        exit(0);
    }

    sys->keyRepeat(true);

    auto vm = std::make_shared<Emulator::VM>(0x003FFFFF);

    uint32_t clockspeed = opt.isSet("-t") ? CLOCK_66MHz_at_60FPS : CLOCK_33MHz_at_60FPS;

    auto debugState = std::make_shared<Client::DebugState>(vm);
    auto emulatorState = std::make_shared<Client::EmulatorState>(vm, program, clockspeed, opt.isSet("-d"));
    auto displayMenuState = std::make_shared<Client::DisplayMenuState>();

    Client::State clientState(
        std::dynamic_pointer_cast<Renderer::Base>(renderer),
        std::dynamic_pointer_cast<Sys::Base>(sys),
        std::pair<uint32_t, std::shared_ptr<Client::BaseState>>(0, std::dynamic_pointer_cast<Client::BaseState>(emulatorState))
    );

    clientState.addState(1, std::dynamic_pointer_cast<Client::BaseState>(displayMenuState));
    clientState.addState(2, std::dynamic_pointer_cast<Client::BaseState>(debugState));

    static uint32_t lastRender = sys->getTicks();
    uint32_t renderTime = sys->getTicks();

    while (sys->handleEvents(clientState)) {
        auto t1 = std::chrono::high_resolution_clock::now();
        sys->clearScreen();
        clientState.tick(renderTime - lastRender);
        auto t2 = std::chrono::high_resolution_clock::now();
        clientState.render(renderTime - lastRender);
        auto t3 = std::chrono::high_resolution_clock::now();

        lastRender = renderTime;

        sys->swapBuffers();
        auto t4 = std::chrono::high_resolution_clock::now();
        auto taken = std::chrono::duration_cast<std::chrono::milliseconds>(t4 - t1).count();

//        std::cerr << std::chrono::duration_cast<std::chrono::milliseconds>(t4 - t1).count() << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(16 - taken));
    }

    exit(0);
}
