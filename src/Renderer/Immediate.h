#ifndef __RENDERER_IMMEDIATE_H__
#define __RENDERER_IMMEDIATE_H__

#include <cstdint>
#include <array>
#include <string>

#include "Common/DisplayMode.h"
#include "Renderer/Base.h"

namespace Renderer {

enum class Mode {
    Interface
};

class Immediate : public Base {
    Font font;
    Mode mode;

    void enableInterfaceMode();

    Common::DisplayMode displayMode;

    int virtualHeight;
    int verticalOffset;
public:
    Immediate(const Common::DisplayMode &displayMode);
    void drawString(const uint16_t x, const uint16_t y, const uint16_t w, const uint16_t h, const std::string &str, const Common::Colour &colour = Common::Colour::White);
    void drawRect(const uint16_t x, const uint16_t y, const uint16_t w, const uint16_t h, const Common::Colour &colour);
    void drawQuad(const Vec2d &a, const Vec2d &b, const Vec2d &c, const Vec2d &d, const Common::Colour &colour);
    void drawPoint(const uint16_t x, const uint16_t y, const Common::Colour &colour, const uint16_t size=1);
    void drawLine(const Vec2d &start, const Vec2d &end, const Common::Colour &colour);
    void drawBuffer(const uint32_t *buffer, uint32_t width, uint32_t height, uint32_t size=1);

    void changeDisplayMode(const Common::DisplayMode &displayMode);

    ~Immediate();
};

}; // namespace Renderer

#endif //__RENDERER_IMMEDIATE_H__

