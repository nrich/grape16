#ifndef __SYS_GLFW_H__
#define __SYS_GLFW_H__

#include <GLFW/glfw3.h>

#include <memory>
#include <functional>
#include <queue>
#include <vector>

#include <portaudio.h>

#include "Sys/Base.h"
#include "Client/State.h"

#include "Audio/Tone.h"

namespace Sys {
    class GLFW : public Base {
            std::shared_ptr<GLFWwindow> window;
            PaStream *stream;
            std::array<Audio::Tone, VOICE_COUNT> voices;
        public:
            GLFW(const std::string &title);
            ~GLFW();
            Common::DisplayMode changeDisplayMode(const Common::DisplayMode &displayMode, bool fullscreen);
            std::vector<Common::DisplayMode> getDisplayModes() const;
            Common::DisplayMode findDisplayMode(uint16_t width, uint16_t height) const;
            Common::DisplayMode currentDisplayMode() const;
            std::pair<Common::DisplayMode, Common::DisplayMode> getPreviousNextMode(const Common::DisplayMode &displayMode) const;
            bool isFullScreen() const;
            bool handleEvents(std::shared_ptr<Client::State> clientState);
        
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

            void sound(uint8_t voice, float frequency, uint16_t duration, int waveForm);
    };
}; // Sys

#endif //__SYS_GLFW_H__
