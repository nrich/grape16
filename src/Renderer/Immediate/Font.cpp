#include <cstdio>
#include <cstdint>

#include <algorithm>

#include <iostream>

#include "Renderer/Immediate/Font.h"

using namespace Renderer;
//using namespace Immediate;

static const char *nulpixelbitmap = 
    "        "
    "        "
    "        "
    "        "
    "        "
    "        "
    "        "
    "        ";
   
static const char *blockpixelbitmap = 
"11111111"
"11111111"
"11111111"
"11111111"
"11111111"
"11111111"
"11111111"
"11111111";


static const char *pixelbitmaps[] = {
    // NUL (0)
    NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
    NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
    NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
    NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,

    // Space (32)
    "        "
    "        "
    "        "
    "        "
    "        "
    "        "
    "        "
    "        ",

    // !
    "        "
    "   1    "
    "   1    "
    "   1    "
    "   1    "
    "        "
    "   1    "
    "        ",
    
    // "
    "        "
    "  1  1  "
    "  1  1  "
    "        "
    "        "
    "        "
    "        "
    "        ",

    // #
    "        "
    "  1 1   "
    " 11111  "
    "  1 1   "
    " 11111  "
    "  1 1   "
    "        "
    "        ",

    // $
    "   1    "
    "  1111  "
    " 1 1    "
    "  111   "
    "   1 1  "
    " 1 1 1  "
    "  111   "
    "   1    ",

    // %
    "        "
    "  11    "
    "  11 1  "
    "    1   "
    "   1    "
    "  1 11  "
    " 1  11  "
    "        ",

    // &
    "        "
    "  11    "
    " 1  1   "
    "  11    "
    " 1 11   "
    " 1  11  "
    "  11  1 "
    "        ",
    // '
    "        "
    "  1     "
    "  1     "
    "        "
    "        "
    "        "
    "        "
    "        ",

    // (
    "        "
    "   1    "
    "  1     "
    "  1     "
    "  1     "
    "  1     "
    "   1    "
    "        ",

    // )
    "        "
    "    1   "
    "     1  "
    "     1  "
    "     1  "
    "     1  "
    "    1   "
    "        ",

    // *
    "        "
    "   1    "
    "   1    "
    " 11111  "
    "  1 1   "
    " 1   1  "
    " 1   1  "
    "        ",

    // +
    "        "
    "   1    "
    "   1    "
    " 11111  "
    "   1    "
    "   1    "
    "        "
    "        ",

    // ,
    "        "
    "        "
    "        "
    "        "
    " 11     "
    " 11     "
    "  1     "
    "        ",

    // -
    "        "
    "        "
    "        "
    " 11111  "
    "        "
    "        "
    "        "
    "        ",

    // .
    "        "
    "        "
    "        "
    "        "
    "        "
    "   11   "
    "   11   "
    "        ",

    // /
    "        "
    "     1  "
    "    1   "
    "   1    "
    "  1     "
    " 1      "
    "        "
    "        ",

    // 0
    "        "
    "  111   "
    " 1   1  "
    " 1 1 1  "
    " 1 1 1  "
    " 1   1  "
    "  111   "
    "        ",

    // 1
    "        "
    "  11    "
    "   1    "
    "   1    "
    "   1    "
    "   1    "
    " 11111  "
    "        ",

    // 2
    "        "
    "  111   "
    " 1   1  "
    "    1   "
    "   1    "
    "  1     "
    " 11111  "
    "        ",

    // 3
    "        "
    "  111   "
    " 1   1  "
    "   11   "
    "     1  "
    " 1   1  "
    "  111   "
    "        ",

    // 4
    "        "
    "    1   "
    "   11   "
    "  1 1   "
    " 11111  "
    "    1   "
    "    1   "
    "        ",

    // 5
    "        "
    " 1111   "
    " 1      "
    " 1111   "
    "     1  "
    " 1   1  "
    "  111   "
    "        ",

    // 6
    "        "
    "  111   "
    " 1      "
    " 1111   "
    " 1   1  "
    " 1   1  "
    "  111   "
    "        ",

    // 7
    "        "
    " 11111  "
    "     1  "
    "    1   "
    "   1    "
    "   1    "
    "   1    "
    "        ",

    // 8
    "        "
    "  111   "
    " 1   1  "
    "  111   "
    " 1   1  "
    " 1   1  "
    "  111   "
    "        ",

    // 9
    "        "
    "  111   "
    " 1   1  "
    " 1   1  "
    "  1111  "
    "     1  "
    "  111   "
    "        ",

    // :
    "        "
    "   11   "
    "   11   "
    "        "
    "        "
    "   11   "
    "   11   "
    "        ",

    // ;
    "        "
    "   11   "
    "   11   "
    "        "
    "        "
    "   11   "
    "  11    "
    "        ",

    // <
    "        "
    "    1   "
    "   1    "
    "  1     "
    "   1    "
    "    1   "
    "        "
    "        ",

    // =
    "        "
    "        "
    " 11111  "
    "        "
    " 11111  "
    "        "
    "        "
    "        ",

    // >
    "        "
    "  1     "
    "   1    "
    "    1   "
    "   1    "
    "  1     "
    "        "
    "        ",

    // ?
    "        "
    "  111   "
    " 1   1  "
    "    1   "
    "   1    "
    "        "
    "   1    "
    "        ",

    // @
    "        "
    "  111   "
    " 1   1  "
    " 1 111  "
    " 1 1 1  "
    " 1 1 1  "
    "  111   "
    "        ",

    // A (65)
    "        "
    "   1    "
    "  1 1   "
    " 1   1  "
    " 11111  "
    " 1   1  "
    " 1   1  "
    "        ",

    // B
    "        "
    " 1111   "
    " 1   1  "
    " 1111   "
    " 1   1  "
    " 1   1  "
    " 1111   "
    "        ",

    // C
    "        "
    "  111   "
    " 1   1  "
    " 1      "
    " 1      "
    " 1   1  "
    "  111   "
    "        ",

    // D
    "        "
    " 1111   "
    " 1   1  "
    " 1   1  "
    " 1   1  "
    " 1   1  "
    " 1111   "
    "        ",

    // E
    "        "
    " 11111  "
    " 1      "
    " 1111   "
    " 1      "
    " 1      "
    " 11111  "
    "        ",

    // F
    "        "
    " 11111  "
    " 1      "
    " 1111   "
    " 1      "
    " 1      "
    " 1      "
    "        ",

    // G
    "        "
    "  111   "
    " 1   1  "
    " 1      "
    " 1  11  "
    " 1   1  "
    "  111   "
    "        ",

    // H
    "        "
    " 1   1  "
    " 1   1  "
    " 11111  "
    " 1   1  "
    " 1   1  "
    " 1   1  "
    "        ",

    // I
    "        "
    " 11111  "
    "   1    "
    "   1    "
    "   1    "
    "   1    "
    " 11111  "
    "        ",

    // J
    "        "
    " 11111  "
    "    1   "
    "    1   "
    " 1  1   "
    " 1  1   "
    "  11    "
    "        ",

    // K
    "        "
    " 1   1  "
    " 1  1   "
    " 111    "
    " 1  1   "
    " 1   1  "
    " 1   1  "
    "        ",

    // L
    "        "
    " 1      "
    " 1      "
    " 1      "
    " 1      "
    " 1      "
    " 11111  "
    "        ",

    // M
    "        "
    " 1   1  "
    " 11 11  "
    " 1 1 1  "
    " 1   1  "
    " 1   1  "
    " 1   1  "
    "        ",

    // N
    "        "
    " 1   1  "
    " 11  1  "
    " 1 1 1  "
    " 1  11  "
    " 1   1  "
    " 1   1  "
    "        ",

    // O
    "        "
    "  111   "
    " 1   1  "
    " 1   1  "
    " 1   1  "
    " 1   1  "
    "  111   "
    "        ",

    // P
    "        "
    " 1111   "
    " 1   1  "
    " 1   1  "
    " 1111   "
    " 1      "
    " 1      "
    "        ",

    // Q
    "        "
    "  111   "
    " 1   1  "
    " 1   1  "
    " 1 1 1  "
    " 1  11  "
    "  111   "
    "        ",

    // R
    "        "
    " 1111   "
    " 1   1  "
    " 1   1  "
    " 1111   "
    " 1  1   "
    " 1   1  "
    "        ",

    // S
    "        "
    "  111   "
    " 1      "
    "  111   "
    "     1  "
    " 1   1  "
    "  111   "
    "        ",

    // T
    "        "
    " 11111  "
    "   1    "
    "   1    "
    "   1    "
    "   1    "
    "   1    "
    "        ",

    // U
    "        "
    " 1   1  "
    " 1   1  "
    " 1   1  "
    " 1   1  "
    " 1   1  "
    "  111   "
    "        ",

    // V
    "        "
    " 1   1  "
    " 1   1  "
    " 1   1  "
    " 1   1  "
    "  1 1   "
    "   1    "
    "        ",

    // W
    "        "
    " 1   1  "
    " 1   1  "
    " 1   1  "
    " 1 1 1  "
    " 1 1 1  "
    "  1 1   "
    "        ",

    // X
    "        "
    " 1   1  "
    "  1 1   "
    "   1    "
    "  1 1   "
    " 1   1  "
    " 1   1  "
    "        ",

    // Y
    "        "
    " 1   1  "
    "  1 1   "
    "   1    "
    "   1    "
    "   1    "
    "   1    "
    "        ",

    // Z
    "        "
    " 11111  "
    "     1  "
    "    1   "
    "   1    "
    "  1     "
    " 11111  "
    "        ",

    // [
    "        "
    " 111    "
    " 1      "
    " 1      "
    " 1      "
    " 1      "
    " 111    "
    "        ",

    // bslash
    "        "
    " 1      "
    "  1     "
    "   1    "
    "    1   "
    "     1  "
    "        "
    "        ",

    // ]
    "        "
    "   111  "
    "     1  "
    "     1  "
    "     1  "
    "     1  "
    "   111  "
    "        ",

    // ^
    "        "
    "   1    "
    "  1 1   "
    " 1   1  "
    "        "
    "        "
    "        "
    "        ",

    // _
    "        "
    "        "
    "        "
    "        "
    "        "
    "        "
    " 11111  "
    "        ",

    // `
    "        "
    "  1     "
    "   1    "
    "        "
    "        "
    "        "
    "        "
    "        ",

    // a (65)
    "        "
    "        "
    "  111   "
    "     1  "
    "  1111  "
    " 1   1  "
    "  1111  "
    "        ",

    // b
    "        "
    " 1      "
    " 1      "
    " 1111   "
    " 1   1  "
    " 1   1  "
    " 1111   "
    "        ",

    // c
    "        "
    "        "
    "  111   "
    " 1   1  "
    " 1      "
    " 1   1  "
    "  111   "
    "        ",

    // d
    "        "
    "     1  "
    "     1  "
    "  1111  "
    " 1   1  "
    " 1   1  "
    "  1111  "
    "        ",

    // e
    "        "
    "        "
    "  111   "
    " 1   1  "
    " 1111   "
    " 1      "
    "  111   "
    "        ",

    // f
    "        "
    "   111  "
    "  1     "
    " 1111   "
    "  1     "
    "  1     "
    "  1     "
    "        ",

    // g
    "        "
    "        "
    "  1111  "
    " 1   1  "
    "  1111  "
    "     1  "
    "  111   "
    "        ",
    // h
    "        "
    " 1      "
    " 1      "
    " 1 11   "
    " 11  1  "
    " 1   1  "
    " 1   1  "
    "        ",

    // i
    "        "
    "   1    "
    "        "
    "   1    "
    "   1    "
    "   1    "
    "   1    "
    "        ",

    // j
    "        "
    "    1   "
    "        "
    "    1   "
    " 1  1   "
    " 1  1   "
    "  11    "
    "        ",

    // k
    "        "
    " 1      "
    " 1      "
    " 1  1   "
    " 111    "
    " 1  1   "
    " 1   1  "
    "        ",

    // l
    "        "
    "  11    "
    "   1    "
    "   1    "
    "   1    "
    "   1    "
    "   11   "
    "        ",

    // m
    "        "
    "        "
    "  1 1   "
    " 1 1 1  "
    " 1   1  "
    " 1   1  "
    " 1   1  "
    "        ",

    // n
    "        "
    "        "
    "  111   "
    " 1   1  "
    " 1   1  "
    " 1   1  "
    " 1   1  "
    "        ",

    // o
    "        "
    "        "
    "  111   "
    " 1   1  "
    " 1   1  "
    " 1   1  "
    "  111   "
    "        ",

    // p
    "        "
    "        "
    " 1111   "
    " 1   1  "
    " 1111   "
    " 1      "
    " 1      "
    "        ",

    // q
    "        "
    "        "
    "  1111  "
    " 1   1  "
    "  1111  "
    "     1  "
    "     1  "
    "        ",

    // r
    "        "
    "        "
    "  111   "
    " 1   1  "
    " 1      "
    " 1      "
    " 1      "
    "        ",

    // s
    "        "
    "        "
    "  111   "
    " 1      "
    "  111   "
    "     1  "
    "  111   "
    "        ",

    // t
    "        "
    "   1    "
    "   1    "
    " 11111  "
    "   1    "
    "   1    "
    "    11  "
    "        ",

    // u
    "        "
    "        "
    " 1   1  "
    " 1   1  "
    " 1   1  "
    " 1   1  "
    "  111   "
    "        ",

    // v
    "        "
    "        "
    " 1   1  "
    " 1   1  "
    " 1   1  "
    "  1 1   "
    "   1    "
    "        ",

    // w
    "        "
    "        "
    " 1   1  "
    " 1   1  "
    " 1   1  "
    " 1 1 1  "
    "  1 1   "
    "        ",

    // x
    "        "
    "        "
    " 1   1  "
    "  1 1   "
    "   1    "
    "  1 1   "
    " 1   1  "
    "        ",

    // y
    "        "
    "        "
    " 1   1  "
    "  1 1   "
    "   1    "
    "   1    "
    "  1     "
    "        ",

    // z
    "        "
    "        "
    " 11111  "
    "    1   "
    "   1    "
    "  1     "
    " 11111  "
    "        ",

    // {
    "        "
    "  11    "
    " 1      "
    "  1     "
    " 1      "
    " 1      "
    "  11    "
    "        ",

    // |
    "        "
    "   1    "
    "   1    "
    "   1    "
    "   1    "
    "   1    "
    "   1    "
    "        ",

    // }
    "        "
    "   11   "
    "     1  "
    "    1   "
    "     1  "
    "     1  "
    "   11   "
    "        ",

    // ~
    "        "
    "        "
    "  1 1   "
    " 1 1    "
    "        "
    "        "
    "        "
    "        ",

    // DEL
    "        "
    "        "
    "        "
    "        "
    "        "
    "        "
    "        "
    "        ",

    NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
    NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
    NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
    NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,

    NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
    NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
    NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
    NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,

    NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
    NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
    NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
    NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,

    NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
    NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
    NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
    NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
};

Font::Font() {
    std::array<char, 64> nul{0};
    for (int i = 0; i < 64; i++) {
        auto pixel = nulpixelbitmap[i] != ' ' ? 255 : 0;
        nul[i] = pixel;
    }
    nulTexture = CreateTexture(nul);

    std::array<char, 64> block{0};
    for (int i = 0; i < 64; i++) {
        auto pixel = blockpixelbitmap[i] != ' ' ? 255 : 0;
        block[i] = pixel;
    }

    blockTexture = CreateTexture(block); 

    for (uint32_t i = 0; i < fontTextures.size(); i++) {
        std::array<char, 64> chr{0};
        auto pixelbitmap = pixelbitmaps[i];
        if (pixelbitmap) {
            for (int i = 0; i < 64; i++) {
                auto pixel = pixelbitmap[i] != ' ' ? 255 : 0;
                chr[i] = pixel;
            }

            fontTextures[i] = CreateTexture(chr);
        } else {
            fontTextures[i] = nulTexture;
        }
    }
}

GLuint Font::CreateTexture(std::array<char, 64> pixels) {
    GLuint texture;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_INTENSITY, 8, 8, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, pixels.data());

    return texture;
}

void Font::drawString(const uint16_t x, const uint16_t y, const uint16_t w, const uint16_t h, const std::string &str) {
    glEnable (GL_BLEND);
    //glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    uint16_t offset = 0;

    glEnable(GL_TEXTURE_2D);

    for (const auto c : str) {
        GLuint fontTexture = fontTextures[c];
        glBindTexture(GL_TEXTURE_2D, fontTexture);

        glBegin(GL_QUADS);
            glTexCoord2f(0, 0);
            glVertex2f(x + (offset*w), y);

            glTexCoord2f(1, 0);
            glVertex2f(x + (offset*w) + w, y);

            glTexCoord2f(1, 1);
            glVertex2f(x + (offset*w) + w, y + h);

            glTexCoord2f(0, 1);
            glVertex2f(x + (offset*w), y + h);
        glEnd();
        offset++;
    }
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
}

Font::~Font() {
    for (uint32_t i = 0; i < fontTextures.size(); i++) {
        auto fontTexture = fontTextures[i];
        glDeleteTextures(1, &fontTexture);
    }
}
