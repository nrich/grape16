#ifndef __RENDERER_IMMEDIATE_FONT_H__
#define __RENDERER_IMMEDIATE_FONT_H__

#include <cstdint>
#include <array>
#include <string>

#include "Common/Shared.h"

namespace Renderer {
//namespace Immediate {

class Font {
    GLuint nulTexture;
    GLuint blockTexture;
    std::array<GLuint, 256> fontTextures;

    static GLuint CreateTexture(std::array<char, 64> pixels);
public:
    Font();
    void drawString(const uint16_t x, const uint16_t y, const uint16_t w, const uint16_t h, const std::string &str);
    ~Font();
};

//}; // namespace Immediate
}; // namespace Renderer 

#endif //__RENDERER_IMMEDIATE_FONT_H__
