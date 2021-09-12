#ifndef __SYS_TERM_H__
#define __SYS_TERM_H__

#include "Sys/Base.h"
#include "Client/State.h"

namespace Sys {
    class Term : public Base {
            void clientKeyDown(Client::State &clientState, const char ch) const;
            void clientKeyUp(Client::State &clientState, const char ch) const;
         public:
            Term(const std::string &title);
            ~Term();
            Common::DisplayMode changeDisplayMode(const Common::DisplayMode &displayMode, bool fullscreen);
            std::vector<Common::DisplayMode> getDisplayModes() const;
            Common::DisplayMode findDisplayMode(uint16_t width, uint16_t height) const;
            Common::DisplayMode currentDisplayMode() const;
            std::pair<Common::DisplayMode, Common::DisplayMode> getPreviousNextMode(const Common::DisplayMode &displayMode) const;
            bool isFullScreen() const {
                return true;
            };
            bool handleEvents(Client::State &clientState) const;
        
            void swapBuffers() const {
            }

            uint32_t getTicks() const {
                return 0;
            }

            void clearScreen() const {
            }
    };
}; // Sys

#endif //__SYS_SDL2_H__
