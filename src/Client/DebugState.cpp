#include "Client/State.h"
#include "Client/DebugState.h"
#include "Common/Keys.h"

using namespace Client;

void DebugState::onRender(State *state, const uint32_t time) {
    auto renderer = state->getRenderer();
}

void DebugState::onTick(State *state, const uint32_t time) {
}

void DebugState::onMouseMove(State *state, const MouseMove &event) {

}

void DebugState::onMouseButtonPress(State *state, const MouseClick &event) {

}

void DebugState::onMouseButtonRelease(State *state, const MouseClick &event) {

}

void DebugState::onKeyDown(State *state, const KeyPress &event) {
    if (event.keyCode == Common::Keys::F2) {
        state->changeState(0);
    } else if (event.keyCode == Common::Keys::Space) {
        vm->run(std::dynamic_pointer_cast<Emulator::SysIO>(sysio), *program, 1, true, true);
    }

}

void DebugState::onKeyUp(State *state, const KeyPress &event) {

}

void DebugState::onEnterState(State *state, std::any data) {
    program = std::any_cast<std::shared_ptr<Emulator::Program>>(data);
}
