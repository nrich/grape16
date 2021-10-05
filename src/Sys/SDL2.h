#ifndef __SYS_SDL2_H__
#define __SYS_SDL2_H__

#include <SDL.h>

#include <memory>
#include <functional>
#include <array>

#include "Sys/Base.h"
#include "Client/State.h"
#include "Audio/Tone.h"

namespace Sys {
    class SDL2 : public Base {
            std::shared_ptr<SDL_Window> window;
            SDL_AudioDeviceID dev;
            SDL_GLContext context;

            void setWindow(std::shared_ptr<SDL_Window> window) {
                this->window = window;
            }

            bool repeatKeys;

            std::array<Audio::Tone, VOICE_COUNT> voices;
        public:
            SDL2(const std::string &title);
            ~SDL2();
            Common::DisplayMode changeDisplayMode(const Common::DisplayMode &displayMode, bool fullscreen);
            std::vector<Common::DisplayMode> getDisplayModes() const;
            Common::DisplayMode findDisplayMode(uint16_t width, uint16_t height) const;
            Common::DisplayMode currentDisplayMode() const;
            std::pair<Common::DisplayMode, Common::DisplayMode> getPreviousNextMode(const Common::DisplayMode &displayMode) const;
            bool isFullScreen() const;
            bool handleEvents(std::shared_ptr<Client::State> clientState);
        
            void swapBuffers() {
                SDL_GL_SwapWindow(window.get());
            }

            uint32_t getTicks() const {
                return SDL_GetTicks();
            }

            void clearScreen() const {
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            }

            void keyRepeat(bool enable) {
                repeatKeys = enable;
            }

            void sound(uint8_t voice, float frequency, uint16_t duration, uint8_t waveForm, uint8_t volume, uint8_t attack, uint8_t decay, uint8_t sustain, uint8_t release);
    };
}; // Sys

#endif //__SYS_SDL2_H__
