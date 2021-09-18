#ifndef __CLIENT_LOADINGSTATE_H__
#define __CLIENT_LOADINGSTATE_H__

#include "Client/BaseState.h"

namespace Client {
    class State;

    class DebugIO : public Emulator::SysIO {
        public:
            DebugIO() {

            }

            void cls() {

            }

            void write(uint8_t c) {

            }

            uint8_t read() {
                return 0;
            }

            void puts(const std::string &str) {

            }

            std::string gets() {
                return "";
            }

            void setpixel(uint16_t x, uint16_t y, uint8_t pixel) {
            }

            uint8_t getpixel(uint16_t x, uint16_t y) {
                return 0;
            }

            void blit(uint16_t x, uint16_t y, std::vector<uint8_t> buffer) {

            }
    };

    class DebugState : public BaseState {
            std::shared_ptr<Emulator::VM> vm;
            std::shared_ptr<Emulator::Program> program;
            std::shared_ptr<DebugIO> sysio;
        public:
            DebugState(std::shared_ptr<Emulator::VM> vm) : vm(vm) {
                sysio = std::make_shared<DebugIO>();
            }

            void onRender(State *state, const uint32_t time);
            void onTick(State *state, const uint32_t time);
            void onMouseMove(State *state, const MouseMove &event);
            void onMouseButtonPress(State *state, const MouseClick &event);
            void onMouseButtonRelease(State *state, const MouseClick &event);
            void onKeyDown(State *state, const KeyPress &event);
            void onKeyUp(State *state, const KeyPress &event);
            void onEnterState(State *state, std::any data);
    };
};

#endif //__CLIENT_LOADINGSTATE_H__
