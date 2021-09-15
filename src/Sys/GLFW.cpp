#include "Sys/GLFW.h"

#include "Common/Keys.h"

#include <algorithm>

#include <iostream>

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Client::State *clientState = (Client::State *)glfwGetWindowUserPointer(window);
    Client::KeyPress event;

    event.shiftMod = mods & GLFW_MOD_SHIFT;
    event.ctrlMod = mods & GLFW_MOD_CONTROL;
    event.altMod = mods & GLFW_MOD_ALT;
    event.guiMod = mods & GLFW_MOD_SUPER;

    if (key == GLFW_KEY_ESCAPE) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    } else if (key == GLFW_KEY_ENTER) {
        event.keyCode = Common::Keys::Enter;
    } else if (key == GLFW_KEY_LEFT_SHIFT) {
        event.keyCode = Common::Keys::LShift;
    } else if (key == GLFW_KEY_A) {
        event.keyCode = Common::Keys::A;
    }

    if (action == GLFW_PRESS) {
        clientState->keyDown(event);
    } else if (action == GLFW_RELEASE) {
        clientState->keyUp(event);
    }
}

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
    glfwSetWindowUserPointer(window.get(), &clientState);
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

    window = std::shared_ptr<GLFWwindow>(
        glfwCreateWindow(mode.Width(), mode.Height(), (std::string("GLFW ") + title).c_str(), NULL, NULL),
        glfwDestroyWindow
    );

    glfwMakeContextCurrent(window.get());

    glfwSwapInterval(1);

    glfwSetKeyCallback(window.get(), key_callback);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

Sys::GLFW::~GLFW() {
    glfwTerminate();
}

