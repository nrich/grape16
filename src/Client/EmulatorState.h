#ifndef __CLIENT_EMULATORSTATE_H__
#define __CLIENT_EMULATORSTATE_H__

#include <stack>

#include "Emulator/VM.h"
#include "Client/BaseState.h"

namespace Client {
    class State;

    class EmulatorState : public BaseState {
            std::shared_ptr<Emulator::VM> vm;
            std::shared_ptr<Emulator::Program> program;
            std::stack<char> keypresses;
            uint32_t clockspeed;
        public:
            EmulatorState(std::shared_ptr<Emulator::VM> vm, std::shared_ptr<Emulator::Program> program, uint32_t clockspeed) : vm(vm), program(program), clockspeed(clockspeed)  {
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
