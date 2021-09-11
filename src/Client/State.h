#ifndef __CLIENT_STATE_H__
#define __CLIENT_STATE_H__

#include <map>
#include <memory>

#include "Client/BaseState.h"

#include "Renderer/Base.h"
#include "Common/DisplayMode.h"

#include "Sys/Base.h"

namespace Client {
    enum class States {
        Loading,
        FirstPerson,
        DisplayMenu,
        Map,
        Count
    };

    class State {
        std::map<States, std::shared_ptr<BaseState>> states;
        std::shared_ptr<Client::BaseState> currentState;
        std::shared_ptr<Renderer::Base> renderer;
        std::shared_ptr<Sys::Base> sys;
//        Common::DisplayMode displayMode;
    public:
        State(std::shared_ptr<Renderer::Base> renderer, std::shared_ptr<Sys::Base> sys);
        void changeState(const States newState, const int enter=0, const int leave=0);

        // Event dispatch
        void render(const uint32_t time);
        void tick(const uint32_t time);
        void mouseMove(const MouseMove &event);
        void mouseButtonPress(const MouseClick &event);
        void mouseButtonRelease(const MouseClick &event);
        void keyDown(const KeyPress &event);
        void keyUp(const KeyPress &event);

        std::shared_ptr<Renderer::Base> getRenderer() const {
            return renderer;
        }

        std::shared_ptr<Sys::Base> getSys() const {
            return sys;
        }

        const Common::DisplayMode getDisplayMode() const {
            return sys->currentDisplayMode();
        }

        void changeDisplayMode(const Common::DisplayMode &mode, bool fullscreen) {
            //displayMode = sys->changeDisplayMode(mode, fullscreen);
            sys->changeDisplayMode(mode, fullscreen);
            renderer->changeDisplayMode(mode);
        }

        ~State();
    };

};

#endif //__CLIENT_STATE_H__
