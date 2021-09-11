#ifndef __SYS_SDL2_H__
#define __SYS_SDL2_H__

#include <SDL.h>

#include <memory>
#include <functional>

#include "Sys/Base.h"
#include "Client/State.h"

namespace Sys {
    class SDL2 : public Base {
            std::shared_ptr<SDL_Window> window;
            SDL_GLContext context;

            void clientKeyDown(Client::State &clientState, const SDL_KeyboardEvent &sdlEvent) const;
            void clientKeyUp(Client::State &clientState, const SDL_KeyboardEvent &sdlEvent) const;
            void clientMouseMove(Client::State &clientState, const SDL_MouseMotionEvent &sdlEvent) const;
            void clientMouseButtonPress(Client::State &clientState, const SDL_MouseButtonEvent &sdlEvent) const;
            void clientMouseButtonRelease(Client::State &clientState, const SDL_MouseButtonEvent &sdlEvent) const;

            void setWindow(std::shared_ptr<SDL_Window> window) {
                this->window = window;
            }
        public:
            SDL2(const std::string &title);
            ~SDL2();
            Common::DisplayMode changeDisplayMode(const Common::DisplayMode &displayMode, bool fullscreen);
            std::vector<Common::DisplayMode> getDisplayModes() const;
            Common::DisplayMode findDisplayMode(uint16_t width, uint16_t height) const;
            Common::DisplayMode currentDisplayMode() const;
            std::pair<Common::DisplayMode, Common::DisplayMode> getPreviousNextMode(const Common::DisplayMode &displayMode) const;
            bool isFullScreen() const;
            bool handleEvents(Client::State &clientState) const;
        
            void swapBuffers() const {
                SDL_GL_SwapWindow(window.get());
            }

            uint32_t getTicks() const {
                return SDL_GetTicks();
            }

            void clearScreen() const {
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            }
    };
}; // Sys

#endif //__SYS_SDL2_H__
