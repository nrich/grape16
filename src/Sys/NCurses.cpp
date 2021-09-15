#include "Sys/NCurses.h"
#include "Common/Keys.h"

#include <algorithm>

#include <iostream>

#include <ncurses.h>

static int _kbhit(void) {
    int ch = getch();

    if (ch != ERR) {
        ungetch(ch);
        return 1;
    } else {
        return 0;
    }
}

void Sys::NCurses::clientKeyDown(Client::State &clientState, const char ch) const {
    Client::KeyPress event;

    event.shiftMod = false;

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
            break;
        case '1':
            event.keyCode = Common::Keys::Num1;
            break;
        case '2':
            event.keyCode = Common::Keys::Num2;
            break;
        case '3':
            event.keyCode = Common::Keys::Num3;
            break;
        case '4':
            event.keyCode = Common::Keys::Num4;
            break;
        case '5':
            event.keyCode = Common::Keys::Num5;
            break;
        case '6':
            event.keyCode = Common::Keys::Num6;
            break;
        case '7':
            event.keyCode = Common::Keys::Num7;
            break;
        case '8':
            event.keyCode = Common::Keys::Num8;
            break;
        case '9':
            event.keyCode = Common::Keys::Num9;
            break;
        case '0':
            event.keyCode = Common::Keys::Num0;
            break;
        case '!':
            event.keyCode = Common::Keys::Num1;
            event.shiftMod = true;
            break;
        case '@':
            event.keyCode = Common::Keys::Num2;
            event.shiftMod = true;
            break;
        case '#':
            event.keyCode = Common::Keys::Num3;
            event.shiftMod = true;
            break;
        case '$':
            event.keyCode = Common::Keys::Num4;
            event.shiftMod = true;
            break;
        case '%':
            event.keyCode = Common::Keys::Num5;
            event.shiftMod = true;
            break;
        case '^':
            event.keyCode = Common::Keys::Num6;
            event.shiftMod = true;
            break;
        case '&':
            event.keyCode = Common::Keys::Num7;
            event.shiftMod = true;
            break;
        case '*':
            event.keyCode = Common::Keys::Num8;
            event.shiftMod = true;
            break;
        case '(':
            event.keyCode = Common::Keys::Num9;
            event.shiftMod = true;
            break;
        case ')':
            event.keyCode = Common::Keys::Num0;
            event.shiftMod = true;
            break;
        case '`':
            event.keyCode = Common::Keys::Backquote;
            break;
        case '~':
            event.keyCode = Common::Keys::Backquote;
            event.shiftMod = true;
            break;
        case '[':
            event.keyCode = Common::Keys::LBracket;
            break;
        case '{':
            event.keyCode = Common::Keys::LBracket;
            event.shiftMod = true;
            break;
        case ']':
            event.keyCode = Common::Keys::RBracket;
            break;
        case '}':
            event.keyCode = Common::Keys::RBracket;
            event.shiftMod = true;
            break;
        case ';':
            event.keyCode = Common::Keys::Semicolon;
            break;
        case ':':
            event.keyCode = Common::Keys::Semicolon;
            event.shiftMod = true;
            break;
        case ',':
            event.keyCode = Common::Keys::Comma;
            break;
        case '<':
            event.keyCode = Common::Keys::Comma;
            event.shiftMod = true;
            break;
        case '.':
            event.keyCode = Common::Keys::Period;
            break;
        case '>':
            event.keyCode = Common::Keys::Period;
            event.shiftMod = true;
            break;
        case '\'':
            event.keyCode = Common::Keys::Quote;
            break;
        case '"':
            event.keyCode = Common::Keys::Quote;
            event.shiftMod = true;
            break;
        case '/':
            event.keyCode = Common::Keys::Slash;
            break;
        case '?':
            event.keyCode = Common::Keys::Slash;
            event.shiftMod = true;
            break;
        case '\\':
            event.keyCode = Common::Keys::Backslash;
            break;
        case '|':
            event.keyCode = Common::Keys::Backslash;
            event.shiftMod = true;
            break;
        case '=':
            event.keyCode = Common::Keys::Equal;
            break;
        case '+':
            event.keyCode = Common::Keys::Equal;
            event.shiftMod = true;
            break;
        case '-':
            event.keyCode = Common::Keys::Hyphen;
            break;
        case '_':
            event.keyCode = Common::Keys::Hyphen;
            event.shiftMod = true;
            break;
        case ' ':
            event.keyCode = Common::Keys::Space;
            break;
        case 8:
            event.keyCode = Common::Keys::Backspace;
            break;
        case '\t':
            event.keyCode = Common::Keys::Tab;
            break;
    }
    
    clientState.keyDown(event);
}

Common::DisplayMode Sys::NCurses::changeDisplayMode(const Common::DisplayMode &displayMode, bool fullscreen) {
    return displayMode;
}

std::vector<Common::DisplayMode> Sys::NCurses::getDisplayModes() const {
    std::vector<Common::DisplayMode> modes;
    modes.push_back(Common::DisplayMode(0, 0, 60));
    return modes;
}

Common::DisplayMode Sys::NCurses::findDisplayMode(uint16_t width, uint16_t height) const {
    auto modeList = getDisplayModes();
    return modeList[0];
}

Common::DisplayMode Sys::NCurses::currentDisplayMode() const {
    return Common::DisplayMode(0, 0, 60);
}

std::pair<Common::DisplayMode, Common::DisplayMode> Sys::NCurses::getPreviousNextMode(const Common::DisplayMode &displayMode) const {
    return std::pair<Common::DisplayMode, Common::DisplayMode>(Common::DisplayMode(0, 0, 60), Common::DisplayMode(0, 0, 60));
}

bool Sys::NCurses::handleEvents(Client::State &clientState) {
    bool run = true;
    while (_kbhit())  {
        char ch = getch();
        clientKeyDown(clientState, ch);
    }

    return run;
}

Sys::NCurses::NCurses(const std::string &title, std::shared_ptr<WINDOW> window) : window(window) {
    //initscr();

    cbreak();
    noecho();
    nodelay(stdscr, TRUE);
    scrollok(stdscr, TRUE);

    curs_set(0);
    clear();

    //box(window.get(), 0, 0);
    //wrefresh(window.get());
}

Sys::NCurses::~NCurses() {
    endwin();
}

