#include "Client/State.h"
#include "Client/DisplayMenuState.h"

#include "Common/Keys.h"

#include <vector>
#include <iostream>

using namespace Client;

enum DisplayMenu {
    MENU_RESOLUTION = 0,
    MENU_FULLSCREEN,
    MENU_COUNT
}; 

void DisplayMenuState::onRender(State *state, const uint32_t time) {
    auto renderer = state->getRenderer();
    auto sys = state->getSys();

    auto currentDisplayMode = state->getDisplayMode();

    if (menuState == DisplayMenu::MENU_RESOLUTION) {
        std::ostringstream s;
        s << "> Screen: " << currentDisplayMode.Width() << "x" << currentDisplayMode.Height() << " @ " << currentDisplayMode.Refresh() << "Hz";
        renderer->drawString(0, 0, 8, 8, s.str(), Common::Colour::White);
    } else {
        std::ostringstream s;
        s << "  Screen: " << currentDisplayMode.Width() << "x" << currentDisplayMode.Height() << " @ " << currentDisplayMode.Refresh() << "Hz";
        renderer->drawString(0, 0, 8, 8, s.str(), Common::Colour::Grey);
    }

    if (menuState == DisplayMenu::MENU_FULLSCREEN) {
        std::ostringstream s;
        s << "> Fullscreen: " << (sys->isFullScreen() ? "true" : "false");
        renderer->drawString(0, 8, 8, 8, s.str(), Common::Colour::White);
    } else {
        std::ostringstream s;
        s << "  Fullscreen: " << (sys->isFullScreen() ? "true" : "false");
        renderer->drawString(0, 8, 8, 8, s.str(), Common::Colour::Grey);
    }
}

void DisplayMenuState::onTick(State *state, const uint32_t time) {
}

void DisplayMenuState::onMouseMove(State *state, const MouseMove &event) {

}

void DisplayMenuState::onMouseButtonPress(State *state, const MouseClick &event) {

}

void DisplayMenuState::onMouseButtonRelease(State *state, const MouseClick &event) {

}

void DisplayMenuState::onEnterState(State *state, std::any data) {
}

void DisplayMenuState::onKeyDown(State *state, const KeyPress &event) {
    auto sys = state->getSys();

    if (event.keyCode == Common::Keys::F1) {
        state->changeState(0);
    } else if (event.keyCode == '1') {
    } else if (event.keyCode == '2') {
    } else if (event.keyCode == '3') {
    } else if (event.keyCode == Common::Keys::Left) {
        if (menuState == DisplayMenu::MENU_RESOLUTION) {
            auto modes = sys->getPreviousNextMode(state->getDisplayMode());
            state->changeDisplayMode(modes.first, sys->isFullScreen());
        } else if (menuState == DisplayMenu::MENU_FULLSCREEN) {
            state->changeDisplayMode(state->getDisplayMode(), false);
        }
    } else if (event.keyCode == Common::Keys::Right) {
        if (menuState == DisplayMenu::MENU_RESOLUTION) {
            auto modes = sys->getPreviousNextMode(state->getDisplayMode());
            state->changeDisplayMode(modes.second, sys->isFullScreen());
        } else if (menuState == DisplayMenu::MENU_FULLSCREEN) {
            state->changeDisplayMode(state->getDisplayMode(), true);
        }
    } else if (event.keyCode == Common::Keys::Up) {
        if (menuState <= 0) {
            menuState = DisplayMenu::MENU_COUNT;
        }

        menuState = (menuState - 1) % DisplayMenu::MENU_COUNT;
    } else if (event.keyCode == Common::Keys::Down) {
        menuState = (menuState + 1) % DisplayMenu::MENU_COUNT;
    }
}

void DisplayMenuState::onKeyUp(State *state, const KeyPress &event) {

}
