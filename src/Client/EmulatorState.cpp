#include "Client/State.h"
#include "Client/EmulatorState.h"
#include "Common/Keys.h"

#include <vector>

using namespace Client;

SystemIO::SystemIO() : cursor(0,0) {
    screenbuffer.resize(lines);
    screenbuffer[cursor.Y()][cursor.X()] = 228;
}

void SystemIO::cls() {
}

void SystemIO::write(uint8_t c) {
    char chr = (char)c;

    if (chr == '\n') {
        cursor = (cursor + Point(0,1)).withX(0);
        chr = 0;
    } else if (chr == 8) {
        if (cursor != Point(0,0)) {
            cursor = (cursor - Point(1,0));
            screenbuffer[cursor.Y()][cursor.X()] = ' ';
        }
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
        screenbuffer.erase(screenbuffer.begin());
        screenbuffer.push_back(std::array<char, chars>());
    }

    if (chr) {
        screenbuffer[cursor.Y()][cursor.X()] = chr;
        cursor = (cursor + Point(1,0));
    } else {
        screenbuffer[cursor.Y()][cursor.X()] = 228;
    }

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
    for (auto line : sysio->getScreenBuffer()) {
        state->getRenderer()->drawString(0, lineoffset*8, 8, 8, std::string(line.data()));
        lineoffset++;
    }
}

void EmulatorState::onTick(State *state, const uint32_t time) {
    static bool done = false;
    static std::string input = "";

    if (!done) {
        done = vm->run(std::dynamic_pointer_cast<Emulator::SysIO>(sysio), *program, clockspeed, false, false);
        return;
    }

    while (auto c = sysio->buffer()) {
        sysio->write(c);

        if (c == '\n') {
            if (input == "LIST") {
                for (auto const b : basic) {
                    sysio->puts(std::to_string(b.first));
                    for (auto token : b.second) {
                        sysio->puts(std::string(" ") + token.str);
                    }
                    sysio->write('\n');
                }
            } else if (input == "RUN") {
                auto run = program->add(Emulator::OpCode::NOP);
                compile(basic, *program);
                vm->Jump(run);
                done = false;
            } else {
                auto basicline = parseLine(input);
                basic[basicline.first] = basicline.second;
            }

            input = "";
        } else {
            input += c;
        }
    }
}

void EmulatorState::onMouseMove(State *state, const MouseMove &event) {
}

void EmulatorState::onMouseButtonPress(State *state, const MouseClick &event) {
}

void EmulatorState::onMouseButtonRelease(State *state, const MouseClick &event) {
}

void EmulatorState::onKeyDown(State *state, const KeyPress &event) {
    if (event.keyCode == Common::Keys::A) {
        sysio->buffer(event.shiftMod ? 'A' : 'a');
    } else if (event.keyCode == Common::Keys::B) {
        sysio->buffer(event.shiftMod ? 'B' : 'b');
    } else if (event.keyCode == Common::Keys::C) {
        sysio->buffer(event.shiftMod ? 'C' : 'c');
    } else if (event.keyCode == Common::Keys::D) {
        sysio->buffer(event.shiftMod ? 'D' : 'd');
    } else if (event.keyCode == Common::Keys::E) {
        sysio->buffer(event.shiftMod ? 'E' : 'e');
    } else if (event.keyCode == Common::Keys::F) {
        sysio->buffer(event.shiftMod ? 'F' : 'f');
    } else if (event.keyCode == Common::Keys::G) {
        sysio->buffer(event.shiftMod ? 'G' : 'g');
    } else if (event.keyCode == Common::Keys::H) {
        sysio->buffer(event.shiftMod ? 'H' : 'h');
    } else if (event.keyCode == Common::Keys::I) {
        sysio->buffer(event.shiftMod ? 'I' : 'i');
    } else if (event.keyCode == Common::Keys::J) {
        sysio->buffer(event.shiftMod ? 'J' : 'j');
    } else if (event.keyCode == Common::Keys::K) {
        sysio->buffer(event.shiftMod ? 'K' : 'k');
    } else if (event.keyCode == Common::Keys::L) {
        sysio->buffer(event.shiftMod ? 'L' : 'l');
    } else if (event.keyCode == Common::Keys::M) {
        sysio->buffer(event.shiftMod ? 'M' : 'm');
    } else if (event.keyCode == Common::Keys::N) {
        sysio->buffer(event.shiftMod ? 'N' : 'n');
    } else if (event.keyCode == Common::Keys::O) {
        sysio->buffer(event.shiftMod ? 'O' : 'o');
    } else if (event.keyCode == Common::Keys::P) {
        sysio->buffer(event.shiftMod ? 'P' : 'p');
    } else if (event.keyCode == Common::Keys::Q) {
        sysio->buffer(event.shiftMod ? 'Q' : 'q');
    } else if (event.keyCode == Common::Keys::R) {
        sysio->buffer(event.shiftMod ? 'R' : 'r');
    } else if (event.keyCode == Common::Keys::S) {
        sysio->buffer(event.shiftMod ? 'S' : 's');
    } else if (event.keyCode == Common::Keys::T) {
        sysio->buffer(event.shiftMod ? 'T' : 't');
    } else if (event.keyCode == Common::Keys::U) {
        sysio->buffer(event.shiftMod ? 'U' : 'u');
    } else if (event.keyCode == Common::Keys::V) {
        sysio->buffer(event.shiftMod ? 'V' : 'v');
    } else if (event.keyCode == Common::Keys::W) {
        sysio->buffer(event.shiftMod ? 'W' : 'w');
    } else if (event.keyCode == Common::Keys::X) {
        sysio->buffer(event.shiftMod ? 'X' : 'x');
    } else if (event.keyCode == Common::Keys::Y) {
        sysio->buffer(event.shiftMod ? 'Y' : 'y');
    } else if (event.keyCode == Common::Keys::Z) {
        sysio->buffer(event.shiftMod ? 'Z' : 'z');
    } else if (event.keyCode == Common::Keys::Num1) {
        sysio->buffer(event.shiftMod ? '!' : '1');
    } else if (event.keyCode == Common::Keys::Num2) {
        sysio->buffer(event.shiftMod ? '@' : '2');
    } else if (event.keyCode == Common::Keys::Num3) {
        sysio->buffer(event.shiftMod ? '#' : '3');
    } else if (event.keyCode == Common::Keys::Num4) {
        sysio->buffer(event.shiftMod ? '$' : '4');
    } else if (event.keyCode == Common::Keys::Num5) {
        sysio->buffer(event.shiftMod ? '%' : '5');
    } else if (event.keyCode == Common::Keys::Num6) {
        sysio->buffer(event.shiftMod ? '^' : '6');
    } else if (event.keyCode == Common::Keys::Num7) {
        sysio->buffer(event.shiftMod ? '&' : '7');
    } else if (event.keyCode == Common::Keys::Num8) {
        sysio->buffer(event.shiftMod ? '*' : '8');
    } else if (event.keyCode == Common::Keys::Num9) {
        sysio->buffer(event.shiftMod ? '(' : '9');
    } else if (event.keyCode == Common::Keys::Num0) {
        sysio->buffer(event.shiftMod ? ')' : '0');
    } else if (event.keyCode == Common::Keys::Backquote) {
        sysio->buffer(event.shiftMod ? '~' : '`');
    } else if (event.keyCode == Common::Keys::LBracket) {
        sysio->buffer(event.shiftMod ? '{' : '[');
    } else if (event.keyCode == Common::Keys::RBracket) {
        sysio->buffer(event.shiftMod ? '}' : ']');
    } else if (event.keyCode == Common::Keys::Semicolon) {
        sysio->buffer(event.shiftMod ? ':' : ';');
    } else if (event.keyCode == Common::Keys::Comma) {
        sysio->buffer(event.shiftMod ? '<' : ',');
    } else if (event.keyCode == Common::Keys::Period) {
        sysio->buffer(event.shiftMod ? '>' : '.');
    } else if (event.keyCode == Common::Keys::Quote) {
        sysio->buffer(event.shiftMod ? '"' : '\'');
    } else if (event.keyCode == Common::Keys::Slash) {
        sysio->buffer(event.shiftMod ? '?' : '/');
    } else if (event.keyCode == Common::Keys::Backslash) {
        sysio->buffer(event.shiftMod ? '|' : '\\');
    } else if (event.keyCode == Common::Keys::Equal) {
        sysio->buffer(event.shiftMod ? '+' : '=');
    } else if (event.keyCode == Common::Keys::Hyphen) {
        sysio->buffer(event.shiftMod ? '_' : '-');
    } else if (event.keyCode == Common::Keys::Enter) {
        sysio->buffer('\n');
    } else if (event.keyCode == Common::Keys::Tab) {
        sysio->buffer('\t');
    } else if (event.keyCode == Common::Keys::Space) {
        sysio->buffer(' ');
    } else if (event.keyCode == Common::Keys::Backspace) {
        sysio->buffer(8);
    } else if (event.keyCode == Common::Keys::F1) {
        state->changeState(1, 0);
    }
}

void EmulatorState::onKeyUp(State *state, const KeyPress &event) {
}
