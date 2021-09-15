#include "Sys/SDL2.h"
#include "Common/Keys.h"

#include <SDL.h>
#include <SDL_opengl.h>

#include <algorithm>

#include <iostream>

static int sym2key(int sym) {
    switch (sym) {
        case SDLK_RETURN:
            return Common::Keys::Enter;
        case SDLK_ESCAPE:
            return Common::Keys::Escape;
        case SDLK_BACKSPACE:
            return Common::Keys::Backspace;
        case SDLK_TAB:
            return Common::Keys::Tab;
        case SDLK_SPACE:
            return Common::Keys::Space;
        case SDLK_EXCLAIM:
            return Common::Keys::Num1;
        case SDLK_QUOTEDBL:
            return Common::Keys::Quote;
        case SDLK_HASH:
            return Common::Keys::Num3;
        case SDLK_PERCENT:
            return Common::Keys::Num5;
        case SDLK_DOLLAR:
            return Common::Keys::Num4;
        case SDLK_AMPERSAND:
            return Common::Keys::Num7;
        case SDLK_QUOTE:
            return Common::Keys::Quote;
        case SDLK_LEFTPAREN:
            return Common::Keys::Num9;
        case SDLK_RIGHTPAREN:
            return Common::Keys::Num0;
        case SDLK_ASTERISK:
            return Common::Keys::Num8;
        case SDLK_PLUS:
            return Common::Keys::Equal;
        case SDLK_COMMA:
            return Common::Keys::Comma;
        case SDLK_MINUS:
            return Common::Keys::Subtract;
        case SDLK_PERIOD:
            return Common::Keys::Period;
        case SDLK_SLASH:
            return Common::Keys::Divide;
        case SDLK_0:
            return Common::Keys::Num0;
        case SDLK_1:
            return Common::Keys::Num1;
        case SDLK_2:
            return Common::Keys::Num2;
        case SDLK_3:
            return Common::Keys::Num3;
        case SDLK_4:
            return Common::Keys::Num4;
        case SDLK_5:
            return Common::Keys::Num5;
        case SDLK_6:
            return Common::Keys::Num6;
        case SDLK_7:
            return Common::Keys::Num7;
        case SDLK_8:
            return Common::Keys::Num8;
        case SDLK_9:
            return Common::Keys::Num9;
        case SDLK_COLON:
            return Common::Keys::Semicolon;
        case SDLK_SEMICOLON:
            return Common::Keys::Semicolon;
        case SDLK_LESS:
            return Common::Keys::Comma;
        case SDLK_EQUALS:
            return Common::Keys::Equal;
        case SDLK_GREATER:
            return Common::Keys::Period;
        case SDLK_QUESTION:
            return Common::Keys::Slash;
        case SDLK_AT:
            return Common::Keys::Num2;
        case SDLK_LEFTBRACKET:
            return Common::Keys::LBracket;
        case SDLK_BACKSLASH:
            return Common::Keys::Backslash;
        case SDLK_RIGHTBRACKET:
            return Common::Keys::RBracket;
        case SDLK_CARET:
            return Common::Keys::Num6;
        case SDLK_UNDERSCORE:
            return Common::Keys::Subtract;
        case SDLK_BACKQUOTE:
            return Common::Keys::Backquote;
        case SDLK_a:
            return Common::Keys::A;
        case SDLK_b:
            return Common::Keys::B;
        case SDLK_c:
            return Common::Keys::C;
        case SDLK_d:
            return Common::Keys::D;
        case SDLK_e:
            return Common::Keys::E;
        case SDLK_f:
            return Common::Keys::F;
        case SDLK_g:
            return Common::Keys::G;
        case SDLK_h:
            return Common::Keys::H;
        case SDLK_i:
            return Common::Keys::I;
        case SDLK_j:
            return Common::Keys::J;
        case SDLK_k:
            return Common::Keys::K;
        case SDLK_l:
            return Common::Keys::L;
        case SDLK_m:
            return Common::Keys::M;
        case SDLK_n:
            return Common::Keys::N;
        case SDLK_o:
            return Common::Keys::O;
        case SDLK_p:
            return Common::Keys::P;
        case SDLK_q:
            return Common::Keys::Q;
        case SDLK_r:
            return Common::Keys::R;
        case SDLK_s:
            return Common::Keys::S;
        case SDLK_t:
            return Common::Keys::T;
        case SDLK_u:
            return Common::Keys::U;
        case SDLK_v:
            return Common::Keys::V;
        case SDLK_w:
            return Common::Keys::W;
        case SDLK_x:
            return Common::Keys::X;
        case SDLK_y:
            return Common::Keys::Y;
        case SDLK_z:
            return Common::Keys::Z;
        case SDLK_F1:
            return Common::Keys::F1;
        case SDLK_F2:
            return Common::Keys::F2;
        case SDLK_F3:
            return Common::Keys::F3;
        case SDLK_F4:
            return Common::Keys::F4;
        case SDLK_F5:
            return Common::Keys::F5;
        case SDLK_F6:
            return Common::Keys::F6;
        case SDLK_F7:
            return Common::Keys::F7;
        case SDLK_F8:
            return Common::Keys::F8;
        case SDLK_F9:
            return Common::Keys::F9;
        case SDLK_F10:
            return Common::Keys::F10;
        case SDLK_F11:
            return Common::Keys::F11;
        case SDLK_F12:
            return Common::Keys::F12;
        case SDLK_F13:
            return Common::Keys::F13;
        case SDLK_F14:
            return Common::Keys::F14;
        case SDLK_F15:
            return Common::Keys::F15;
        case SDLK_PAUSE:
            return Common::Keys::Pause;
        case SDLK_INSERT:
            return Common::Keys::Insert;
        case SDLK_HOME:
            return Common::Keys::Home;
        case SDLK_PAGEUP:
            return Common::Keys::PageUp;
        case SDLK_DELETE:
            return Common::Keys::Delete;
        case SDLK_END:
            return Common::Keys::End;
        case SDLK_PAGEDOWN:
            return Common::Keys::PageDown;
        case SDLK_RIGHT:
            return Common::Keys::Right;
        case SDLK_LEFT:
            return Common::Keys::Left;
        case SDLK_DOWN:
            return Common::Keys::Down;
        case SDLK_UP:
            return Common::Keys::Up;
        case SDLK_KP_DIVIDE:
            return Common::Keys::Divide;
        case SDLK_KP_MULTIPLY:
            return Common::Keys::Multiply;
        case SDLK_KP_MINUS:
            return Common::Keys::Subtract;
        case SDLK_KP_PLUS:
            return Common::Keys::Add;
        case SDLK_KP_ENTER:
            return Common::Keys::Enter;
        case SDLK_KP_1:
            return Common::Keys::Numpad1;
        case SDLK_KP_2:
            return Common::Keys::Numpad2;
        case SDLK_KP_3:
            return Common::Keys::Numpad3;
        case SDLK_KP_4:
            return Common::Keys::Numpad4;
        case SDLK_KP_5:
            return Common::Keys::Numpad5;
        case SDLK_KP_6:
            return Common::Keys::Numpad6;
        case SDLK_KP_7:
            return Common::Keys::Numpad7;
        case SDLK_KP_8:
            return Common::Keys::Numpad8;
        case SDLK_KP_9:
            return Common::Keys::Numpad9;
        case SDLK_KP_0:
            return Common::Keys::Numpad0;
        case SDLK_KP_PERIOD:
            return Common::Keys::Period;
        case SDLK_LCTRL:
            return Common::Keys::LControl;
        case SDLK_LSHIFT:
            return Common::Keys::LShift;
        case SDLK_LALT:
            return Common::Keys::LAlt;
        case SDLK_LGUI:
            return Common::Keys::LSystem;
        case SDLK_RCTRL:
            return Common::Keys::RControl;
        case SDLK_RSHIFT:
            return Common::Keys::RShift;
        case SDLK_RALT:
            return Common::Keys::RAlt;
        case SDLK_RGUI:
            return Common::Keys::RSystem;
        case SDLK_MENU:
            return Common::Keys::Menu;
        default:
            return Common::Keys::Unknown;
    }
}

void Sys::SDL2::clientKeyDown(Client::State &clientState, const SDL_KeyboardEvent &sdlEvent) const {
    Client::KeyPress event;

    event.keyCode = sym2key(sdlEvent.keysym.sym);
    event.shiftMod = sdlEvent.keysym.mod & KMOD_SHIFT;
    event.ctrlMod = sdlEvent.keysym.mod & KMOD_CTRL;
    event.altMod = sdlEvent.keysym.mod & KMOD_ALT;
    event.guiMod = sdlEvent.keysym.mod & KMOD_GUI;

    clientState.keyDown(event);
}

void Sys::SDL2::clientKeyUp(Client::State &clientState, const SDL_KeyboardEvent &sdlEvent) const {
    Client::KeyPress event;

    event.keyCode = sym2key(sdlEvent.keysym.sym);
    event.shiftMod = sdlEvent.keysym.mod & KMOD_SHIFT;
    event.ctrlMod = sdlEvent.keysym.mod & KMOD_CTRL;
    event.altMod = sdlEvent.keysym.mod & KMOD_ALT;
    event.guiMod = sdlEvent.keysym.mod & KMOD_GUI;

    clientState.keyUp(event);
}

void Sys::SDL2::clientMouseMove(Client::State &clientState, const SDL_MouseMotionEvent &sdlEvent) const {
    Client::MouseMove event;

    event.x = sdlEvent.x;
    event.y = sdlEvent.y;
    event.xrel = sdlEvent.xrel;
    event.yrel = sdlEvent.yrel;
/*
    event.leftPressed = sdlEvent.state & SDL_BUTTON_LMASK;
    event.middlePressed = sdlEvent.state & SDL_BUTTON_MMASK;
    event.rightPressed = sdlEvent.state & SDL_BUTTON_RMASK;
    event.x1Pressed = sdlEvent.state & SDL_BUTTON_X1MASK;
    event.x2Pressed = sdlEvent.state & SDL_BUTTON_X2MASK;
*/

    clientState.mouseMove(event);
}

void Sys::SDL2::clientMouseButtonPress(Client::State &clientState, const SDL_MouseButtonEvent &sdlEvent) const {
    Client::MouseClick event;

    event.x = sdlEvent.x;
    event.y = sdlEvent.y;
    event.leftPressed = sdlEvent.state & SDL_BUTTON_LMASK;
    event.middlePressed = sdlEvent.state & SDL_BUTTON_MMASK;
    event.rightPressed = sdlEvent.state & SDL_BUTTON_RMASK;
    event.x1Pressed = sdlEvent.state & SDL_BUTTON_X1MASK;
    event.x2Pressed = sdlEvent.state & SDL_BUTTON_X2MASK;

    clientState.mouseButtonPress(event);
}

void Sys::SDL2::clientMouseButtonRelease(Client::State &clientState, const SDL_MouseButtonEvent &sdlEvent) const {
    Client::MouseClick event;

    event.x = sdlEvent.x;
    event.y = sdlEvent.y;
    event.leftPressed = sdlEvent.state & SDL_BUTTON_LMASK;
    event.middlePressed = sdlEvent.state & SDL_BUTTON_MMASK;
    event.rightPressed = sdlEvent.state & SDL_BUTTON_RMASK;
    event.x1Pressed = sdlEvent.state & SDL_BUTTON_X1MASK;
    event.x2Pressed = sdlEvent.state & SDL_BUTTON_X2MASK;

    clientState.mouseButtonRelease(event);
}


Common::DisplayMode Sys::SDL2::changeDisplayMode(const Common::DisplayMode &displayMode, bool fullscreen) {
    if (fullscreen) { 
        if (isFullScreen()) {
            SDL_SetWindowFullscreen(window.get(), 0);
        }

        SDL_DisplayMode mode = {SDL_PIXELFORMAT_UNKNOWN, displayMode.Width(), displayMode.Height(), displayMode.Refresh(), 0};
        SDL_SetWindowDisplayMode(window.get(), &mode);

        if (SDL_SetWindowFullscreen(window.get(), SDL_WINDOW_FULLSCREEN)) {
            SDL_SetWindowFullscreen(window.get(), SDL_WINDOW_FULLSCREEN_DESKTOP);
        }
    } else {
        if (isFullScreen()) {
            SDL_SetWindowFullscreen(window.get(), 0);
        }

        SDL_SetWindowSize(window.get(), displayMode.Width(), displayMode.Height());
    }

    return displayMode;
}

std::vector<Common::DisplayMode> Sys::SDL2::getDisplayModes() const {
    std::vector<Common::DisplayMode> modes;
    int primayDisplay = 0;
    int modeCount = SDL_GetNumVideoDisplays();
    if (modeCount > 1) {
        primayDisplay = modeCount-1;
    }

    for (int i = 0; i < SDL_GetNumDisplayModes(primayDisplay); i++) {
        SDL_DisplayMode mode = {SDL_PIXELFORMAT_UNKNOWN, 0, 0, 0, 0};

        if (SDL_GetDisplayMode(primayDisplay, i, &mode) != 0) {
            SDL_Log("SDL_GetDisplayMode failed: %s", SDL_GetError());
            continue;
        }

        Common::DisplayMode displayMode(mode.w, mode.h, mode.refresh_rate);

        if (displayMode.Ratio() != Common::AspectRatio::Ignore) { 
            modes.push_back(displayMode);
        }
    }

    if (modes.front().Width() > modes.back().Width()) {
        std::reverse(modes.begin(), modes.end());
    }
    return modes;
}

Common::DisplayMode Sys::SDL2::findDisplayMode(uint16_t width, uint16_t height) const {
    auto modeList = getDisplayModes();

    for (auto mode : modeList) {
        if (mode.Width() == width && mode.Height() == height) {
            return mode;
        }
    }

    return modeList[0];
}

Common::DisplayMode Sys::SDL2::currentDisplayMode() const {
    uint32_t flags = SDL_GetWindowFlags(window.get());
    SDL_DisplayMode mode = {SDL_PIXELFORMAT_UNKNOWN, 0, 0, 0, 0};


    if (flags & (SDL_WINDOW_FULLSCREEN|SDL_WINDOW_FULLSCREEN_DESKTOP)) {
        SDL_GetWindowDisplayMode(window.get(), &mode);
        return Common::DisplayMode(mode.w, mode.h, mode.refresh_rate ? mode.refresh_rate : 60);
    } else {
        SDL_GetWindowSize(window.get(), &mode.w, &mode.h);
        return Common::DisplayMode(mode.w, mode.h, mode.refresh_rate ? mode.refresh_rate : 60);
    }
}

std::pair<Common::DisplayMode, Common::DisplayMode> Sys::SDL2::getPreviousNextMode(const Common::DisplayMode &displayMode) const {
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

bool Sys::SDL2::isFullScreen() const {
    uint32_t flags = SDL_GetWindowFlags(window.get());

    return (flags & (SDL_WINDOW_FULLSCREEN|SDL_WINDOW_FULLSCREEN_DESKTOP));
}

bool Sys::SDL2::handleEvents(Client::State &clientState) {
    SDL_Event event;
    bool run = true;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_KEYDOWN:
                if (event.key.repeat == 0)
                    clientKeyDown(clientState, event.key);
                break;
            case SDL_KEYUP:
                if (event.key.repeat == 0)
                    clientKeyUp(clientState, event.key);
                break;
            case SDL_MOUSEBUTTONDOWN:
                clientMouseButtonPress(clientState, event.button);
                break;
            case SDL_MOUSEBUTTONUP:
                clientMouseButtonRelease(clientState, event.button);
                break;
            case SDL_MOUSEMOTION:
                clientMouseMove(clientState, event.motion);
                break;
            case SDL_QUIT:
                run = false;
                break;
            default:
                break;
        }
    }

    return run;
}

Sys::SDL2::SDL2(const std::string &title) {
    SDL_Init(SDL_INIT_EVERYTHING);

    auto mode = getDisplayModes().front();

    auto window = std::shared_ptr<SDL_Window>( 
        SDL_CreateWindow((std::string("SDL2 ") + title).c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, mode.Width(), mode.Height(), SDL_WINDOW_OPENGL),
        SDL_DestroyWindow
    ); 

    setWindow(window);

    SDL_CaptureMouse(SDL_TRUE);
    context = SDL_GL_CreateContext(window.get());

    // Set our OpenGL version.
    // SDL_GL_CONTEXT_CORE gives us only the newer version, deprecated functions are disabled
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // 3.2 is part of the modern versions of OpenGL, but most video cards whould be able to run it
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    // Turn on double buffering with a 24bit Z buffer.
    // You may need to change this to 16 or 32 for your system
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    if (SDL_GL_SetSwapInterval(-1) == -1)
        SDL_GL_SetSwapInterval(1);

    //SDL_SetWindowFullscreen(window.get(), SDL_WINDOW_FULLSCREEN_DESKTOP);
    //SDL_SetWindowFullscreen(window.get(), SDL_WINDOW_FULLSCREEN);
    SDL_GL_SetSwapInterval(0);

    int value = 0;
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &value);
    std::cout << "SDL_GL_CONTEXT_MAJOR_VERSION : " << value << std::endl;

    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &value);
    std::cout << "SDL_GL_CONTEXT_MINOR_VERSION: " << value << std::endl;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

Sys::SDL2::~SDL2() {
    SDL_GL_DeleteContext(context);
    SDL_Quit();
}

