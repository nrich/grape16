#ifndef __RENDERER_NCURSES_H__
#define __RENDERER_NCURSES_H__

#include <cstdint>
#include <array>
#include <string>
#include <memory>

#include <ncurses.h>

#include "Common/DisplayMode.h"
#include "Renderer/Base.h"

namespace Renderer {

class NCurses : public Base {
    std::shared_ptr<WINDOW> window;
public:
    NCurses(std::shared_ptr<WINDOW> window);
    void drawString(const uint16_t x, const uint16_t y, const uint16_t w, const uint16_t h, const std::string &str, const Common::Colour &colour = Common::Colour::White);
    void drawRect(const uint16_t x, const uint16_t y, const uint16_t w, const uint16_t h, const Common::Colour &colour);
    void drawQuad(const Vec2d &a, const Vec2d &b, const Vec2d &c, const Vec2d &d, const Common::Colour &colour);
    void drawPoint(const uint16_t x, const uint16_t y, const Common::Colour &colour, const uint16_t size=1);
    void drawLine(const Vec2d &start, const Vec2d &end, const Common::Colour &colour);
    void drawBuffer(const uint32_t *buffer, uint32_t width, uint32_t height, uint32_t size=1);
    void drawBuffer(const uint8_t *buffer, uint32_t width, uint32_t height, uint32_t size=1);

    void changeDisplayMode(const Common::DisplayMode &displayMode);

    ~NCurses();
};

}; // namespace Renderer

#endif //__RENDERER_NCURSES_H__

