#ifndef __RENDERER_IMMEDIATE_H__
#define __RENDERER_IMMEDIATE_H__

#include <cstdint>
#include <array>
#include <string>

#include "Common/DisplayMode.h"
#include "Renderer/Base.h"

namespace Renderer {

class Immediate : public Base {
    enum class Mode {
        Unknown,
        Interface
    };


    Font font;
    Mode mode;

    void enableInterfaceMode();

    void configureVirtualDisplay();

    Common::DisplayMode displayMode;
    const Common::AspectRatio ratio;

    int virtualHeight;
    int verticalOffset;

    int virtualWidth;
    int horizontalOffset;

    int resolutionScale;
public:
    Immediate(const Common::DisplayMode &displayMode, Common::AspectRatio ratio=Common::AspectRatio::_4x3, int resolutionScale=1);
    void drawString(const uint16_t x, const uint16_t y, const uint16_t w, const uint16_t h, const std::string &str, const Common::Colour &colour = Common::Colour::White);
    void drawRect(const uint16_t x, const uint16_t y, const uint16_t w, const uint16_t h, const Common::Colour &colour);
    void drawQuad(const Vec2d &a, const Vec2d &b, const Vec2d &c, const Vec2d &d, const Common::Colour &colour);
    void drawPoint(const uint16_t x, const uint16_t y, const Common::Colour &colour, const uint16_t size=1);
    void drawLine(const Vec2d &start, const Vec2d &end, const Common::Colour &colour);
    void drawBuffer(const uint32_t *buffer, uint32_t width, uint32_t height, uint32_t size=1);
    void drawBuffer(const uint8_t *buffer, uint32_t width, uint32_t height, uint32_t size=1);

    void changeDisplayMode(const Common::DisplayMode &displayMode);

    ~Immediate();
};

}; // namespace Renderer

#endif //__RENDERER_IMMEDIATE_H__

