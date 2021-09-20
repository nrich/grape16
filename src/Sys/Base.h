#ifndef __SYS_BASE_H__
#define __SYS_BASE_H__

#include <cstdint>
#include <vector>
#include <utility>

#include "Common/Shared.h"
#include "Common/DisplayMode.h"

namespace Client {
    class State;
};

namespace Sys {
    class Base {
        protected:
            bool fullscreen;
        public:
            virtual Common::DisplayMode changeDisplayMode(const Common::DisplayMode &displayMode, bool fullscreen) = 0;
            virtual std::vector<Common::DisplayMode> getDisplayModes() const = 0;
            virtual Common::DisplayMode findDisplayMode(uint16_t width, uint16_t height) const = 0;
            virtual Common::DisplayMode currentDisplayMode() const = 0;
            virtual std::pair<Common::DisplayMode, Common::DisplayMode> getPreviousNextMode(const Common::DisplayMode &displayMode) const = 0;
            virtual bool isFullScreen() const = 0;
            virtual uint32_t getTicks() const = 0;
            virtual void clearScreen() const = 0;
            virtual void swapBuffers() = 0;
            virtual bool handleEvents(Client::State &clientState) = 0;
            virtual void keyRepeat(bool enable) = 0;
    };
}; // Sys

#endif //__SYS_BASE_H__
