#ifndef __SYS_SFML_H__
#define __SYS_SFML_H__

#include <SFML/Graphics.hpp>

#include <memory>
#include <functional>
#include <array>

#include <portaudio.h>

#include "Sys/Base.h"
#include "Client/State.h"
#include "Audio/Tone.h"

namespace Sys {
    class SFML : public Base {
            sf::RenderWindow window;
            sf::Clock clock;

            const std::string title;
            bool isFullscreen;
            PaStream *stream;
            std::array<Audio::Tone, VOICE_COUNT> voices;
        public:
            SFML(const std::string &title);
            ~SFML();
            Common::DisplayMode changeDisplayMode(const Common::DisplayMode &displayMode, bool fullscreen);
            std::vector<Common::DisplayMode> getDisplayModes() const;
            Common::DisplayMode findDisplayMode(uint16_t width, uint16_t height) const;
            Common::DisplayMode currentDisplayMode() const;
            std::pair<Common::DisplayMode, Common::DisplayMode> getPreviousNextMode(const Common::DisplayMode &displayMode) const;
            bool isFullScreen() const;
            bool handleEvents(std::shared_ptr<Client::State> clientState);
        
            void swapBuffers() {
                window.display();
            }

            uint32_t getTicks() const {
                return (uint32_t)clock.getElapsedTime().asMilliseconds();
            }

            void clearScreen() const {
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            }

            void keyRepeat(bool enable) {
                window.setKeyRepeatEnabled(enable);
            }

            void sound(uint8_t voice, float frequency, uint16_t duration, uint8_t waveForm, uint8_t volume, uint8_t attack, uint8_t decay, uint8_t sustain, uint8_t release);
    };
}; // Sys

#endif //__SYS_SFML_H__
