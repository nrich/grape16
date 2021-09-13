#include "Client/State.h"
#include "Client/EmulatorState.h"

#include <vector>

using namespace Client;

void EmulatorState::onRender(State *state, const uint32_t time) {
}

void EmulatorState::onTick(State *state, const uint32_t time) {
    while(vm->run(*program, clockspeed, false, true))
        ;
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
