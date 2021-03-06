#ifndef __SYS_NCURSES_H__
#define __SYS_NCURSES_H__

#include "Sys/Base.h"
#include "Client/State.h"

#include <ncurses.h>

#include <ctime>
#include <ratio>
#include <chrono>

namespace Sys {
    class NCurses : public Base {
            std::shared_ptr<WINDOW> window;
            const std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
         public:
            NCurses(const std::string &title, std::shared_ptr<WINDOW> window);
            ~NCurses();
            Common::DisplayMode changeDisplayMode(const Common::DisplayMode &displayMode, bool fullscreen);
            std::vector<Common::DisplayMode> getDisplayModes() const;
            Common::DisplayMode findDisplayMode(uint16_t width, uint16_t height) const;
            Common::DisplayMode currentDisplayMode() const;
            std::pair<Common::DisplayMode, Common::DisplayMode> getPreviousNextMode(const Common::DisplayMode &displayMode) const;
            bool isFullScreen() const {
                return true;
            };
            bool handleEvents(std::shared_ptr<Client::State> clientState);
        
            void swapBuffers() {
                box(window.get(), 0, 0);
                wrefresh(window.get());
                //refresh();
            }

            uint32_t getTicks() const;

            void clearScreen() const {
                //clear();
                wclear(window.get());
            }

            void sound(uint8_t voice, float frequency, uint16_t duration, uint8_t waveForm, uint8_t volume, uint8_t attack, uint8_t decay, uint8_t sustain, uint8_t release) {
                if (frequency == 800.0f && duration == 250) {
                    beep();
                }
            }

            void keyRepeat(bool enable) {
            }
    };
}; // Sys

#endif //__SYS_NCURSES_H__
