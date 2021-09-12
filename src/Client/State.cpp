#include "Client/State.h"

#include "Client/LoadingState.h"
#include "Client/DisplayMenuState.h"

using namespace Client;

State::State(std::shared_ptr<Renderer::Base> renderer, std::shared_ptr<Sys::Base> sys, std::pair<uint32_t, std::shared_ptr<BaseState>> &state) : renderer(renderer), sys(sys) {
    //states[States::Loading] = std::dynamic_pointer_cast<BaseState>(std::make_shared<LoadingState>());
    //states[States::Emulator] = std::dynamic_pointer_cast<BaseState>(std::make_shared<EmulatorState>(vm));
    //states[States::DisplayMenu] = std::dynamic_pointer_cast<BaseState>(std::make_shared<DisplayMenuState>());

    states[state.first] = state.second;
    currentState = state.second;
}

void State::changeState(const uint32_t newState, const int enter, const int leave) {
    currentState->onLeaveState(this, leave);
    currentState = states[newState];
    currentState->onEnterState(this, enter);
}

void State::render(const uint32_t time) {
    currentState->onRender(this, time);
}

void State::tick(const uint32_t time) {
    currentState->onTick(this, time);
}

void State::mouseMove(const MouseMove &event) {
    currentState->onMouseMove(this, event);
}

void State::mouseButtonPress(const MouseClick &event) {
    currentState->onMouseButtonPress(this, event);
}

void State::mouseButtonRelease(const MouseClick &event) {
    currentState->onMouseButtonRelease(this, event);
}

void State::keyDown(const KeyPress &event) {
    currentState->onKeyDown(this, event);
}

void State::keyUp(const KeyPress &event) {
    currentState->onKeyUp(this, event);
}

State::~State() {

}
