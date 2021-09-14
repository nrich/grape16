#include "Renderer/NCurses.h"

#include <iostream>

using namespace Renderer;

NCurses::NCurses(std::shared_ptr<WINDOW> window) : window(window) {
}

void NCurses::drawString(const uint16_t x, const uint16_t y, const uint16_t w, const uint16_t h, const std::string &str, const Common::Colour &colour) {
    //box(window.get(), 0, 0);
    mvwprintw(window.get(), x+1, y+1, str.c_str());
    //wrefresh(window.get());
    //refresh();
}

void NCurses::drawRect(const uint16_t x, const uint16_t y, const uint16_t w, const uint16_t h, const Common::Colour &colour) {
}

void NCurses::drawQuad(const Vec2d &a, const Vec2d &b, const Vec2d &c, const Vec2d &d, const Common::Colour &colour) {
}

void NCurses::drawLine(const Vec2d &start, const Vec2d &end, const Common::Colour &colour) {
}

void NCurses::drawPoint(const uint16_t x, const uint16_t y, const Common::Colour &colour, const uint16_t size) {
}

void NCurses::drawBuffer(const uint32_t *buffer, uint32_t width, uint32_t height, uint32_t size) {
}

void NCurses::changeDisplayMode(const Common::DisplayMode &displayMode) {
}

NCurses::~NCurses() {
}
