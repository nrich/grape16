#ifndef __CLIENT_DISPLAYMENUSTATE_H__
#define __CLIENT_DISPLAYMENUSTATE_H__

#include "Client/BaseState.h"

namespace Client {
    class State;

    class DisplayMenuState : public BaseState {
            int menuState;
        public:
            DisplayMenuState() : menuState(0) {}

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

#endif //__CLIENT_DISPLAYMENUSTATE_H__
