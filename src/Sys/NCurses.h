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
            void clientKeyDown(Client::State &clientState, const int ch) const;
            void clientKeyUp(Client::State &clientState, const int ch) const;

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
            bool handleEvents(Client::State &clientState);
        
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

            void keyRepeat(bool enable) {
            }
    };
}; // Sys

#endif //__SYS_NCURSES_H__
