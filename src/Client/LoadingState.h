#ifndef __CLIENT_LOADINGSTATE_H__
#define __CLIENT_LOADINGSTATE_H__

#include "Client/BaseState.h"

namespace Client {
    class State;

    class LoadingState : public BaseState {
            float angle;
        public:
            LoadingState() : angle(0.0f) {
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

#endif //__CLIENT_LOADINGSTATE_H__
