#include "Client/BaseState.h"

#include "Client/State.h" 

using namespace Client;

void BaseState::onRender(State *state, const uint32_t time) {

}

void BaseState::onTick(State *state, const uint32_t time) {

}

void BaseState::onMouseMove(State *state, const MouseMove &event) {

}

void BaseState::onMouseButtonPress(State *state, const MouseClick &event) {

}

void BaseState::onMouseButtonRelease(State *state, const MouseClick &event) {

}

void BaseState::onKeyDown(State *state, const KeyPress &event) {

}

void BaseState::onKeyUp(State *state, const KeyPress &event) {

}

void BaseState::onEnterState(State *state, const int data) {

}

void BaseState::onLeaveState(State *state, const int data) {

}


void BaseState::changeDisplayMode(const Common::DisplayMode &displayMode) {

}

