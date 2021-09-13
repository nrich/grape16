#include "Client/State.h"
#include "Client/EmulatorState.h"

#include <vector>

using namespace Client;

SystemIO::SystemIO() {
    linebuffer.resize(lines);
}

void SystemIO::cls() {
}

void SystemIO::write(uint8_t c) {
}

uint8_t SystemIO::read() {
    return 0;
}

void SystemIO::puts(const std::string &str) {
    for (auto c : str) {
        if (c == '\n') {
            cursor = (cursor + Point(0,1)).withX(0);
            continue;
        }

        std::cerr << cursor.toString() << std::endl;

        linebuffer[cursor.Y()][cursor.X()] = c;

        cursor = cursor + Point(1, 0);
    }
}

std::string SystemIO::gets() {
    return "";
}


void EmulatorState::onRender(State *state, const uint32_t time) {
    uint16_t lineoffset = 0;
    for (auto line : sysio->getLineBuffer()) {
        state->getRenderer()->drawString(0, lineoffset*8, 8, 8, std::string(line.data()));
        lineoffset++;
    }
}

void EmulatorState::onTick(State *state, const uint32_t time) {
    static bool done = false;

    if (!done)
        done = vm->run(std::dynamic_pointer_cast<Emulator::SysIO>(sysio), *program, clockspeed, false, false);
}

void EmulatorState::onMouseMove(State *state, const MouseMove &event) {
}

void EmulatorState::onMouseButtonPress(State *state, const MouseClick &event) {
}

void EmulatorState::onMouseButtonRelease(State *state, const MouseClick &event) {
}

void EmulatorState::onKeyDown(State *state, const KeyPress &event) {
}

void EmulatorState::onKeyUp(State *state, const KeyPress &event) {
}
