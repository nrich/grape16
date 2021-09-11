#include "Client/State.h"
#include "Client/LoadingState.h"

#include <vector>

using namespace Client;

void LoadingState::onRender(State *state, const uint32_t time) {
    auto renderer = state->getRenderer();
}

void LoadingState::onTick(State *state, const uint32_t time) {
    angle += 0.05;
}

void LoadingState::onMouseMove(State *state, const MouseMove &event) {

}

void LoadingState::onMouseButtonPress(State *state, const MouseClick &event) {

}

void LoadingState::onMouseButtonRelease(State *state, const MouseClick &event) {

}

void LoadingState::onKeyDown(State *state, const KeyPress &event) {

}

void LoadingState::onKeyUp(State *state, const KeyPress &event) {

}
