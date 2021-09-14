#ifndef __SYS_NCURSES_H__
#define __SYS_NCURSES_H__

#include "Sys/Base.h"
#include "Client/State.h"

#include <ncurses.h>

namespace Sys {
    class NCurses : public Base {
            void clientKeyDown(Client::State &clientState, const char ch) const;
            void clientKeyUp(Client::State &clientState, const char ch) const;

            std::shared_ptr<WINDOW> window;
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

            uint32_t getTicks() const {
                return 0;
            }

            void clearScreen() const {
                //clear();
            }
    };
}; // Sys

#endif //__SYS_NCURSES_H__
