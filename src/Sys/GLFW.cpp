#include "Sys/GLFW.h"

#include <algorithm>

#include <iostream>

Common::DisplayMode Sys::GLFW::changeDisplayMode(const Common::DisplayMode &displayMode, bool fullscreen) {
    return currentDisplayMode();
}

std::vector<Common::DisplayMode> Sys::GLFW::getDisplayModes() const {
    std::vector<Common::DisplayMode> modes;

    GLFWmonitor *primary = glfwGetPrimaryMonitor();

    int count;
    const GLFWvidmode* vidmodes = glfwGetVideoModes(primary, &count);

    for (int i = 0; i < count; i++) {
        auto mode = vidmodes[i];
        Common::DisplayMode displayMode(mode.width, mode.height, mode.refreshRate);

        if (displayMode.Ratio() != Common::AspectRatio::Ignore) {
            modes.push_back(displayMode);
            std::cerr << mode.width << "x" << mode.height << "@" << mode.refreshRate << std::endl;
        }

    }

    if (modes.front().Width() > modes.back().Width()) {
        std::reverse(modes.begin(), modes.end());
    }
    return modes;
}

Common::DisplayMode Sys::GLFW::findDisplayMode(uint16_t width, uint16_t height) const {
    auto modeList = getDisplayModes();

    for (auto mode : modeList) {
        if (mode.Width() == width && mode.Height() == height) {
            return mode;
        }
    }

    return modeList[0];
}

Common::DisplayMode Sys::GLFW::currentDisplayMode() const {
    int width, height;
    glfwGetFramebufferSize(window.get(), &width, &height);
    return findDisplayMode(width, height);
}

std::pair<Common::DisplayMode, Common::DisplayMode> Sys::GLFW::getPreviousNextMode(const Common::DisplayMode &displayMode) const {
    auto modeList = getDisplayModes();
    Common::DisplayMode previous = displayMode;
    Common::DisplayMode next = displayMode;

    size_t i = 0;
    for (auto mode : modeList) {
        if (mode == displayMode)
            break;

        i++;
    }

    if (i > 0)
        previous = modeList[i-1];
    if (i != modeList.size())
        next = modeList[i+1];
    return std::pair<Common::DisplayMode, Common::DisplayMode>(previous, next);
}

bool Sys::GLFW::isFullScreen() const {
    return false;
}

bool Sys::GLFW::handleEvents(Client::State &clientState) {
    glfwPollEvents();
    return !glfwWindowShouldClose(window.get());
}

static void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

Sys::GLFW::GLFW(const std::string &title) {
    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);

    auto mode = getDisplayModes().front();

    std::cerr << mode.Width() << "x" <<  mode.Height() << std::endl;

    window = std::shared_ptr<GLFWwindow>(
        glfwCreateWindow(mode.Width(), mode.Height(), (std::string("GLFW ") + title).c_str(), NULL, NULL),
        glfwDestroyWindow
    );

    glfwSetWindowUserPointer(window.get(), this);

    glfwMakeContextCurrent(window.get());

    int width, height;
    glfwGetFramebufferSize(window.get(), &width, &height);
    glViewport(0, 0, width, height);

    std::cerr << width << "X" <<  height << std::endl;

    glfwSwapInterval(1);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

Sys::GLFW::~GLFW() {
    glfwTerminate();
}

