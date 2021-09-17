#ifndef __CLIENT_EMULATORSTATE_H__
#define __CLIENT_EMULATORSTATE_H__

#include <queue>

#include "Math/Point2.h"
#include "Emulator/VM.h"
#include "Emulator/Basic.h"
#include "Client/BaseState.h"

namespace Client {
    class State;

    class SystemIO : public Emulator::SysIO {
            const static int32_t chars = 40;
            const static int32_t lines = 22;

            Point cursor;

            std::vector<std::array<char, chars>> screenbuffer;
            std::array<uint8_t, 320*180> screen;

            std::queue<char> inputBuffer;
        public:
            SystemIO();

            void cls();

            void write(uint8_t c);

            uint8_t read();

            void puts(const std::string &str);

            std::string gets();

            std::vector<std::array<char, chars>> getScreenBuffer() {
                return screenbuffer;
            }

            void buffer(char c) {
                inputBuffer.push(c);
            }

            char buffer() {
                if (inputBuffer.size() == 0)
                    return 0;

                char c = inputBuffer.front();
                inputBuffer.pop();
                return c;
            }

            void setpixel(uint16_t x, uint16_t y, uint8_t pixel);
            uint8_t getpixel(uint16_t x, uint16_t y);

            std::array<uint8_t, 320*180> getScreen() {
                return screen;
            }
    };

    class EmulatorState : public BaseState {
            std::shared_ptr<Emulator::VM> vm;
            std::shared_ptr<Emulator::Program> program;
            std::stack<char> keypresses;
            uint32_t clockspeed;
            std::shared_ptr<SystemIO> sysio;

            std::map<uint32_t, std::vector<Emulator::BasicToken>> basic;
            const bool debug;
        public:
            EmulatorState(std::shared_ptr<Emulator::VM> vm, std::shared_ptr<Emulator::Program> program, uint32_t clockspeed, bool debug) : vm(vm), program(program), clockspeed(clockspeed), debug(debug) {
                sysio = std::make_shared<SystemIO>();
            }

            void onRender(State *state, const uint32_t time);
            void onTick(State *state, const uint32_t time);
            void onMouseMove(State *state, const MouseMove &event);
            void onMouseButtonPress(State *state, const MouseClick &event);
            void onMouseButtonRelease(State *state, const MouseClick &event);
            void onKeyDown(State *state, const KeyPress &event);
            void onKeyUp(State *state, const KeyPress &event);
    };
};

#endif //__CLIENT_EMULATORSTATE_H__
