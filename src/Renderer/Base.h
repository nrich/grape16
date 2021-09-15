#ifndef __RENDERER_BASE_H__
#define __RENDERER_BASE_H__

#include <cstdint>
#include <string>
#include <map>
#include <vector>

#include "Math/Point2.h"
#include "Renderer/Immediate/Font.h"
#include "Common/Colour.h"
#include "Common/DisplayMode.h"

namespace Renderer {

class Base {
public:
    Base() {}

    virtual void drawString(const uint16_t x, const uint16_t y, const uint16_t w, const uint16_t h, const std::string &str, const Common::Colour &colour = Common::Colour::White) = 0;
    virtual void drawRect(const uint16_t x, const uint16_t y, const uint16_t w, const uint16_t h, const Common::Colour &colour) = 0;
    virtual void drawQuad(const Vec2d &a, const Vec2d &b, const Vec2d &c, const Vec2d &d, const Common::Colour &colour) = 0;
    virtual void drawPoint(const uint16_t x, const uint16_t y, const Common::Colour &colour, const uint16_t size=1) = 0;
    virtual void drawLine(const Vec2d &start, const Vec2d &end, const Common::Colour &colour) = 0;
    virtual void drawBuffer(const uint32_t *buffer, uint32_t width, uint32_t height, uint32_t size=1) = 0;
    virtual void drawBuffer(const uint8_t *buffer, uint32_t width, uint32_t height, uint32_t size=1) = 0;

    virtual void changeDisplayMode(const Common::DisplayMode &displayMode) = 0;
};

}; // namespace Renderer

#endif //__RENDERER_BASE_H__

