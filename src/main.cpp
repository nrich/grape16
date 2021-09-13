#include <cstdlib>
#include <memory>
#include <functional>
#include <map>

#include <iostream>

#include <ctime>
#include <ratio>
#include <chrono>
#include <thread>

#include "ezOptionParser.hpp"

#include "Client/State.h"

#include "Renderer/Immediate.h"
#include "Renderer/Text.h"

#include "Common/DisplayMode.h"

#include "Sys/SDL2.h"
#include "Sys/SFML.h"
#include "Sys/Term.h"

#include "Emulator/VM.h"
#include "Emulator/Basic.h"
#include "Client/EmulatorState.h"

#define CLOCK_33MHz_at_60FPS  550
#define CLOCK_66MHz_at_60FPS  1100

class TermSysIO : public Emulator::SysIO {
    public:
        TermSysIO() {
        }

        void cls() {
        }

        void write(uint8_t c) {
            std::putchar(c);
        }

        uint8_t read() {
            auto c = std::getchar();
            return c;
        }

        void puts(const std::string &str) {
            std::cout << str << std::endl;
        }

        std::string gets() {
            std::string str;
            std::cin >> str;
            return str;
        }
};

class SystemIO : public Emulator::SysIO {
    public:
        SystemIO() {
        }

        void cls() {
        }

        void write(uint8_t c) {
            std::putchar(c);
        }

        uint8_t read() {
            auto c = std::getchar();
            return c;
        }

        void puts(const std::string &str) {
            std::cout << str << std::endl;
        }

        std::string gets() {
            std::string str;
            std::cin >> str;
            return str;
        }
};


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

int main(int argc, char *argv[]) {
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
        "term", // Default.
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

/*
    Program basic;

                        uint32_t ok = mp;
    auto start      =   basic.addPointer(OpCode::SETIDX, ok, "START");
                        basic.addString(OpCode::SDATA, "OK\n");
                        mp += 4;
                        uint32_t prompt_buffer = mp;
                        basic.addPointer(OpCode::SETIDX, prompt_buffer);
                        basic.addString(OpCode::SDATA, std::string(80, ' '));
    auto prompt     =   basic.addPointer(OpCode::SETIDX, (vmpointer_t)ok);
                        basic.addShort(OpCode::CALL, printfn);
                        basic.addPointer(OpCode::SETIDX, (vmpointer_t)prompt_buffer);
                        basic.addShort(OpCode::CALL, inputfn);
                        basic.addShort(OpCode::IRQ, 0);
                        basic.addShort(OpCode::JMP, prompt);
*/

    std::shared_ptr<Emulator::Program> program;

    if (opt.lastArgs.size() == 1) {
        if (opt.isSet("-b")) {
            program = loadBasic(*opt.lastArgs[0], opt.isSet("-d"));
        } else {
            program = loadAssembly(*opt.lastArgs[0]);
        }
    } else {
        std::cout << "TODO make CLI work" << std::endl;
        exit(0);
    }

    std::shared_ptr<Renderer::Base> renderer;
    std::shared_ptr<Sys::Base> sys;
    std::string sysname;
    opt.get("-s")->getString(sysname);

    if (sysname == "sdl2") {
        sys = std::make_shared<Sys::SDL2>("Grape16");
        renderer = std::make_shared<Renderer::Immediate>(sys->currentDisplayMode());
    } else if (sysname == "sfml") {
        sys = std::make_shared<Sys::SFML>("Grape16");
        renderer = std::make_shared<Renderer::Immediate>(sys->currentDisplayMode());
    } else if (sysname == "term") {
        sys = std::make_shared<Sys::Term>("Grape16");
        renderer = std::make_shared<Renderer::Text>();
    }

    auto sysio = std::make_shared<TermSysIO>();
    auto vm = std::make_shared<Emulator::VM>(std::dynamic_pointer_cast<Emulator::SysIO>(sysio), 0x003FFFFF);

    uint32_t clockspeed = opt.isSet("-t") ? CLOCK_66MHz_at_60FPS : CLOCK_33MHz_at_60FPS;

    auto emulatorState = std::make_shared<Client::EmulatorState>(vm, program, clockspeed);

    //auto renderer = std::make_shared<Renderer::Immediate>(sys->currentDisplayMode());
    //auto renderer = std::make_shared<Renderer::Text>();
    Client::State clientState(
        std::dynamic_pointer_cast<Renderer::Base>(renderer),
        std::dynamic_pointer_cast<Sys::Base>(sys),
        std::pair<uint32_t, std::shared_ptr<Client::BaseState>>(0, std::dynamic_pointer_cast<Client::BaseState>(emulatorState))
    );

    static uint32_t lastRender = sys->getTicks();
    uint32_t renderTime = sys->getTicks();

    while (sys->handleEvents(clientState)) {
        sys->clearScreen();
        //clientState.render(renderTime - lastRender);
        clientState.tick(renderTime - lastRender);
        lastRender = renderTime;

        sys->swapBuffers();
    }

    exit(0);
}
