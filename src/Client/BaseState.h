#ifndef __CLIENT_BASESTATE_H__
#define __CLIENT_BASESTATE_H__

#include <cstdint>
#include <memory>
#include <any>

#include "Client/Events.h"

#include "Renderer/Base.h"

#include "Common/DisplayMode.h"

namespace Client {
    class State;

    class BaseState {
        public:
            virtual void onRender(State *state, const uint32_t time);
            virtual void onTick(State *state, const uint32_t time);
            virtual void onMouseMove(State *state, const MouseMove &event);
            virtual void onMouseButtonPress(State *state, const MouseClick &event);
            virtual void onMouseButtonRelease(State *state, const MouseClick &event);
            virtual void onKeyDown(State *state, const KeyPress &event);
            virtual void onKeyUp(State *state, const KeyPress &event);
            virtual void onEnterState(State *state, std::any data);
            virtual void onLeaveState(State *state, std::any data);

            virtual void changeDisplayMode(const Common::DisplayMode &displayMode);

            virtual ~BaseState();
    };
};

#endif //__CLIENT_BASESTATE_H__
