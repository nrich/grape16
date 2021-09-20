#ifndef __SYS_GLFW_H__
#define __SYS_GLFW_H__

#include <GLFW/glfw3.h>

#include <memory>
#include <functional>

#include "Sys/Base.h"
#include "Client/State.h"

namespace Sys {
    class GLFW : public Base {
            std::shared_ptr<GLFWwindow> window;
        public:
            GLFW(const std::string &title);
            ~GLFW();
            Common::DisplayMode changeDisplayMode(const Common::DisplayMode &displayMode, bool fullscreen);
            std::vector<Common::DisplayMode> getDisplayModes() const;
            Common::DisplayMode findDisplayMode(uint16_t width, uint16_t height) const;
            Common::DisplayMode currentDisplayMode() const;
            std::pair<Common::DisplayMode, Common::DisplayMode> getPreviousNextMode(const Common::DisplayMode &displayMode) const;
            bool isFullScreen() const;
            bool handleEvents(Client::State &clientState);
        
            void swapBuffers() {
                glfwSwapBuffers(window.get());
            }

            uint32_t getTicks() const {
                return (uint32_t)(glfwGetTime()*1000);
            }

            void clearScreen() const {
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            }

            void keyRepeat(bool enable);
    };
}; // Sys

#endif //__SYS_GLFW_H__
