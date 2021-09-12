#include "Sys/Term.h"
#include "Common/Keys.h"

#include <algorithm>

#include <iostream>

#ifdef _WIN32
#include <conio.h>

#else

#include <cstdio>
#include <sys/select.h>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>

int _kbhit() {
    static const int STDIN = 0;
    static bool initialized = false;

    if (! initialized) {
        // Use termios to turn off line buffering
        termios term;
        tcgetattr(STDIN, &term);
        term.c_lflag &= ~ICANON;
        tcsetattr(STDIN, TCSANOW, &term);
        setbuf(stdin, NULL);
        initialized = true;
    }

    int bytesWaiting;
    ioctl(STDIN, FIONREAD, &bytesWaiting);
    return bytesWaiting;
}
#endif

void Sys::Term::clientKeyDown(Client::State &clientState, const char ch) const {
    Client::KeyPress event;

    switch (ch) {
        case '\n':
            event.keyCode = Common::Keys::Enter;
            break;
        case 27:
            event.keyCode = Common::Keys::Escape;
            break;
        case 'a':
            event.keyCode = Common::Keys::A;
            break;
        case 'b':
            event.keyCode = Common::Keys::B;
            break;
        case 'c':
            event.keyCode = Common::Keys::C;
            break;
        case 'd':
            event.keyCode = Common::Keys::D;
            break;
        case 'e':
            event.keyCode = Common::Keys::E;
            break;
        case 'f':
            event.keyCode = Common::Keys::F;
            break;
        case 'g':
            event.keyCode = Common::Keys::G;
            break;
        case 'h':
            event.keyCode = Common::Keys::H;
            break;
        case 'i':
            event.keyCode = Common::Keys::I;
            break;
        case 'j':
            event.keyCode = Common::Keys::J;
            break;
        case 'k':
            event.keyCode = Common::Keys::K;
            break;
        case 'l':
            event.keyCode = Common::Keys::L;
            break;
        case 'm':
            event.keyCode = Common::Keys::M;
            break;
        case 'n':
            event.keyCode = Common::Keys::N;
            break;
        case 'o':
            event.keyCode = Common::Keys::O;
            break;
        case 'p':
            event.keyCode = Common::Keys::P;
            break;
        case 'q':
            event.keyCode = Common::Keys::Q;
            break;
        case 'r':
            event.keyCode = Common::Keys::R;
            break;
        case 's':
            event.keyCode = Common::Keys::S;
            break;
        case 't':
            event.keyCode = Common::Keys::T;
            break;
        case 'u':
            event.keyCode = Common::Keys::U;
            break;
        case 'v':
            event.keyCode = Common::Keys::V;
            break;
        case 'w':
            event.keyCode = Common::Keys::W;
            break;
        case 'x':
            event.keyCode = Common::Keys::X;
            break;
        case 'y':
            event.keyCode = Common::Keys::Y;
            break;
        case 'z':
            event.keyCode = Common::Keys::Z;
            break;
        case 'A':
            event.keyCode = Common::Keys::A;
            event.shiftMod = true;
            break;
        case 'B':
            event.keyCode = Common::Keys::B;
            event.shiftMod = true;
            break;
        case 'C':
            event.keyCode = Common::Keys::C;
            event.shiftMod = true;
            break;
        case 'D':
            event.keyCode = Common::Keys::D;
            event.shiftMod = true;
            break;
        case 'E':
            event.keyCode = Common::Keys::E;
            event.shiftMod = true;
            break;
        case 'F':
            event.keyCode = Common::Keys::F;
            event.shiftMod = true;
            break;
        case 'G':
            event.keyCode = Common::Keys::G;
            event.shiftMod = true;
            break;
        case 'H':
            event.keyCode = Common::Keys::H;
            event.shiftMod = true;
            break;
        case 'I':
            event.keyCode = Common::Keys::I;
            event.shiftMod = true;
            break;
        case 'J':
            event.keyCode = Common::Keys::J;
            event.shiftMod = true;
            break;
        case 'K':
            event.keyCode = Common::Keys::K;
            event.shiftMod = true;
            break;
        case 'L':
            event.keyCode = Common::Keys::L;
            event.shiftMod = true;
            break;
        case 'M':
            event.keyCode = Common::Keys::M;
            event.shiftMod = true;
            break;
        case 'N':
            event.keyCode = Common::Keys::N;
            event.shiftMod = true;
            break;
        case 'O':
            event.keyCode = Common::Keys::O;
            event.shiftMod = true;
            break;
        case 'P':
            event.keyCode = Common::Keys::P;
            event.shiftMod = true;
            break;
        case 'Q':
            event.keyCode = Common::Keys::Q;
            event.shiftMod = true;
            break;
        case 'R':
            event.keyCode = Common::Keys::R;
            event.shiftMod = true;
            break;
        case 'S':
            event.keyCode = Common::Keys::S;
            event.shiftMod = true;
            break;
        case 'T':
            event.keyCode = Common::Keys::T;
            event.shiftMod = true;
            break;
        case 'U':
            event.keyCode = Common::Keys::U;
            event.shiftMod = true;
            break;
        case 'V':
            event.keyCode = Common::Keys::V;
            event.shiftMod = true;
            break;
        case 'W':
            event.keyCode = Common::Keys::W;
            event.shiftMod = true;
            break;
        case 'X':
            event.keyCode = Common::Keys::X;
            event.shiftMod = true;
            break;
        case 'Y':
            event.keyCode = Common::Keys::Y;
            event.shiftMod = true;
            break;
        case 'Z':
            event.keyCode = Common::Keys::Z;
            event.shiftMod = true;
            break;
    }
    
    clientState.keyDown(event);
}

/*
static int sym2key(int sym) {
    switch (sym) {
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
            return Common::Keys::Tilde;
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
*/

void Sys::Term::clientKeyUp(Client::State &clientState, const char ch) const {
/*
    Client::KeyPress event;

    event.keyCode = sym2key(sdlEvent.keysym.sym);
    event.shiftMod = sdlEvent.keysym.mod & KMOD_SHIFT;
    event.ctrlMod = sdlEvent.keysym.mod & KMOD_CTRL;
    event.altMod = sdlEvent.keysym.mod & KMOD_ALT;
    event.guiMod = sdlEvent.keysym.mod & KMOD_GUI;

    clientState.keyUp(event);
*/
}

/*
void Sys::Term::clientMouseMove(Client::State &clientState, const SDL_MouseMotionEvent &sdlEvent) const {
    Client::MouseMove event;

    clientState.mouseMove(event);
}

void Sys::Term::clientMouseButtonPress(Client::State &clientState, const SDL_MouseButtonEvent &sdlEvent) const {
    Client::MouseClick event;

    clientState.mouseButtonPress(event);
}

void Sys::Term::clientMouseButtonRelease(Client::State &clientState, const SDL_MouseButtonEvent &sdlEvent) const {
    Client::MouseClick event;

    clientState.mouseButtonRelease(event);
}
*/


Common::DisplayMode Sys::Term::changeDisplayMode(const Common::DisplayMode &displayMode, bool fullscreen) {
    return displayMode;
}

std::vector<Common::DisplayMode> Sys::Term::getDisplayModes() const {
    std::vector<Common::DisplayMode> modes;
    modes.push_back(Common::DisplayMode(0, 0, 60));
    return modes;
}

Common::DisplayMode Sys::Term::findDisplayMode(uint16_t width, uint16_t height) const {
    auto modeList = getDisplayModes();
    return modeList[0];
}

Common::DisplayMode Sys::Term::currentDisplayMode() const {
    return Common::DisplayMode(0, 0, 60);
}

std::pair<Common::DisplayMode, Common::DisplayMode> Sys::Term::getPreviousNextMode(const Common::DisplayMode &displayMode) const {
    return std::pair<Common::DisplayMode, Common::DisplayMode>(Common::DisplayMode(0, 0, 60), Common::DisplayMode(0, 0, 60));
}

bool Sys::Term::handleEvents(Client::State &clientState) const {
    bool run = true;
    if (!_kbhit())  {
        char ch = std::getchar();
        clientKeyDown(clientState, ch);
    }

    return run;
}

Sys::Term::Term(const std::string &title) {
}

Sys::Term::~Term() {
}

