#include "Renderer/Text.h"

#include <iostream>

using namespace Renderer;

Text::Text() {

}

void Text::drawString(const uint16_t x, const uint16_t y, const uint16_t w, const uint16_t h, const std::string &str, const Common::Colour &colour) {
    std::cout << str;
}

void Text::drawRect(const uint16_t x, const uint16_t y, const uint16_t w, const uint16_t h, const Common::Colour &colour) {
}

void Text::drawQuad(const Vec2d &a, const Vec2d &b, const Vec2d &c, const Vec2d &d, const Common::Colour &colour) {
}

void Text::drawLine(const Vec2d &start, const Vec2d &end, const Common::Colour &colour) {
}

void Text::drawPoint(const uint16_t x, const uint16_t y, const Common::Colour &colour, const uint16_t size) {
}

void Text::drawBuffer(const uint32_t *buffer, uint32_t width, uint32_t height, uint32_t size) {
}

void Text::changeDisplayMode(const Common::DisplayMode &displayMode) {
}

Text::~Text() {
}
