#include "Client/State.h"
#include "Client/EmulatorState.h"
#include "Common/Keys.h"

#include <vector>

using namespace Client;

SystemIO::SystemIO() : cursor(0,0) {
    linebuffer.resize(lines);
}

void SystemIO::cls() {
}

void SystemIO::write(uint8_t c) {
    char chr = (char)c;

    if (chr == '\n') {
        cursor = (cursor + Point(0,1)).withX(0);
        return;
    } else if (chr == '\t') {
        write(' ');
        write(' ');
        return;
    }

    if (cursor.X() >= chars)
        cursor = (cursor + Point(0,1)).withX(0);

    if (cursor.Y() >= lines) {
        cursor = cursor - Point(0, 1);
        linebuffer.erase(linebuffer.begin());
        linebuffer.push_back(std::array<char, chars>());
    }

    linebuffer[cursor.Y()][cursor.X()] = c;
    cursor = (cursor + Point(1,0));
}

uint8_t SystemIO::read() {
    if (inputBuffer.size() == 0)
        return 0;

    uint8_t c = (uint8_t)inputBuffer.front();

    write(c);

    inputBuffer.pop();

    return c;
}

void SystemIO::puts(const std::string &str) {
    for (auto c : str) {
        write(c);
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

    //sysio->puts("OK\n");
}

void EmulatorState::onMouseMove(State *state, const MouseMove &event) {
}

void EmulatorState::onMouseButtonPress(State *state, const MouseClick &event) {
}

void EmulatorState::onMouseButtonRelease(State *state, const MouseClick &event) {
}

void EmulatorState::onKeyDown(State *state, const KeyPress &event) {
    if (event.keyCode == Common::Keys::A) {
        sysio->inputBuffer.push(event.shiftMod ? 'A' : 'a');
    } else if (event.keyCode == Common::Keys::B) {
        sysio->inputBuffer.push(event.shiftMod ? 'B' : 'b');
    } else if (event.keyCode == Common::Keys::C) {
        sysio->inputBuffer.push(event.shiftMod ? 'C' : 'c');
    } else if (event.keyCode == Common::Keys::D) {
        sysio->inputBuffer.push(event.shiftMod ? 'D' : 'd');
    } else if (event.keyCode == Common::Keys::E) {
        sysio->inputBuffer.push(event.shiftMod ? 'E' : 'e');
    } else if (event.keyCode == Common::Keys::F) {
        sysio->inputBuffer.push(event.shiftMod ? 'F' : 'f');
    } else if (event.keyCode == Common::Keys::G) {
        sysio->inputBuffer.push(event.shiftMod ? 'G' : 'g');
    } else if (event.keyCode == Common::Keys::H) {
        sysio->inputBuffer.push(event.shiftMod ? 'H' : 'h');
    } else if (event.keyCode == Common::Keys::I) {
        sysio->inputBuffer.push(event.shiftMod ? 'I' : 'i');
    } else if (event.keyCode == Common::Keys::J) {
        sysio->inputBuffer.push(event.shiftMod ? 'J' : 'j');
    } else if (event.keyCode == Common::Keys::K) {
        sysio->inputBuffer.push(event.shiftMod ? 'K' : 'k');
    } else if (event.keyCode == Common::Keys::L) {
        sysio->inputBuffer.push(event.shiftMod ? 'L' : 'l');
    } else if (event.keyCode == Common::Keys::M) {
        sysio->inputBuffer.push(event.shiftMod ? 'M' : 'm');
    } else if (event.keyCode == Common::Keys::N) {
        sysio->inputBuffer.push(event.shiftMod ? 'N' : 'n');
    } else if (event.keyCode == Common::Keys::O) {
        sysio->inputBuffer.push(event.shiftMod ? 'O' : 'o');
    } else if (event.keyCode == Common::Keys::P) {
        sysio->inputBuffer.push(event.shiftMod ? 'P' : 'p');
    } else if (event.keyCode == Common::Keys::Q) {
        sysio->inputBuffer.push(event.shiftMod ? 'Q' : 'q');
    } else if (event.keyCode == Common::Keys::R) {
        sysio->inputBuffer.push(event.shiftMod ? 'R' : 'r');
    } else if (event.keyCode == Common::Keys::S) {
        sysio->inputBuffer.push(event.shiftMod ? 'S' : 's');
    } else if (event.keyCode == Common::Keys::T) {
        sysio->inputBuffer.push(event.shiftMod ? 'T' : 't');
    } else if (event.keyCode == Common::Keys::U) {
        sysio->inputBuffer.push(event.shiftMod ? 'U' : 'u');
    } else if (event.keyCode == Common::Keys::V) {
        sysio->inputBuffer.push(event.shiftMod ? 'V' : 'v');
    } else if (event.keyCode == Common::Keys::W) {
        sysio->inputBuffer.push(event.shiftMod ? 'W' : 'w');
    } else if (event.keyCode == Common::Keys::X) {
        sysio->inputBuffer.push(event.shiftMod ? 'X' : 'x');
    } else if (event.keyCode == Common::Keys::Y) {
        sysio->inputBuffer.push(event.shiftMod ? 'Y' : 'y');
    } else if (event.keyCode == Common::Keys::Z) {
        sysio->inputBuffer.push(event.shiftMod ? 'Z' : 'z');
    } else if (event.keyCode == Common::Keys::Num1) {
        sysio->inputBuffer.push(event.shiftMod ? '!' : '1');
    } else if (event.keyCode == Common::Keys::Num2) {
        sysio->inputBuffer.push(event.shiftMod ? '@' : '2');
    } else if (event.keyCode == Common::Keys::Num3) {
        sysio->inputBuffer.push(event.shiftMod ? '#' : '3');
    } else if (event.keyCode == Common::Keys::Num4) {
        sysio->inputBuffer.push(event.shiftMod ? '$' : '4');
    } else if (event.keyCode == Common::Keys::Num5) {
        sysio->inputBuffer.push(event.shiftMod ? '%' : '5');
    } else if (event.keyCode == Common::Keys::Num6) {
        sysio->inputBuffer.push(event.shiftMod ? '^' : '6');
    } else if (event.keyCode == Common::Keys::Num7) {
        sysio->inputBuffer.push(event.shiftMod ? '&' : '7');
    } else if (event.keyCode == Common::Keys::Num8) {
        sysio->inputBuffer.push(event.shiftMod ? '*' : '8');
    } else if (event.keyCode == Common::Keys::Num9) {
        sysio->inputBuffer.push(event.shiftMod ? '(' : '9');
    } else if (event.keyCode == Common::Keys::Num0) {
        sysio->inputBuffer.push(event.shiftMod ? ')' : '0');
    } else if (event.keyCode == Common::Keys::Backquote) {
        sysio->inputBuffer.push(event.shiftMod ? '~' : '`');
    } else if (event.keyCode == Common::Keys::LBracket) {
        sysio->inputBuffer.push(event.shiftMod ? '{' : '[');
    } else if (event.keyCode == Common::Keys::RBracket) {
        sysio->inputBuffer.push(event.shiftMod ? '}' : ']');
    } else if (event.keyCode == Common::Keys::Semicolon) {
        sysio->inputBuffer.push(event.shiftMod ? ':' : ';');
    } else if (event.keyCode == Common::Keys::Comma) {
        sysio->inputBuffer.push(event.shiftMod ? '<' : ',');
    } else if (event.keyCode == Common::Keys::Period) {
        sysio->inputBuffer.push(event.shiftMod ? '>' : '.');
    } else if (event.keyCode == Common::Keys::Quote) {
        sysio->inputBuffer.push(event.shiftMod ? '"' : '\'');
    } else if (event.keyCode == Common::Keys::Slash) {
        sysio->inputBuffer.push(event.shiftMod ? '?' : '/');
    } else if (event.keyCode == Common::Keys::Backslash) {
        sysio->inputBuffer.push(event.shiftMod ? '|' : '\\');
    } else if (event.keyCode == Common::Keys::Equal) {
        sysio->inputBuffer.push(event.shiftMod ? '+' : '=');
    } else if (event.keyCode == Common::Keys::Hyphen) {
        sysio->inputBuffer.push(event.shiftMod ? '_' : '-');
    } else if (event.keyCode == Common::Keys::Enter) {
        sysio->inputBuffer.push('\n');
    } else if (event.keyCode == Common::Keys::Tab) {
        sysio->inputBuffer.push('\t');
    } else if (event.keyCode == Common::Keys::Space) {
        sysio->inputBuffer.push(' ');
    } else if (event.keyCode == Common::Keys::Backspace) {
        sysio->inputBuffer.push(8);
    } else if (event.keyCode == Common::Keys::F1) {
        state->changeState(1, 0);
    }
}

void EmulatorState::onKeyUp(State *state, const KeyPress &event) {
}
