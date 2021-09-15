#include "Renderer/Immediate.h"

#include <GL/glu.h>
#include <iostream>


#define GL_RESCALE_NORMAL                       0x803A
#define GL_CLAMP_TO_EDGE                        0x812F
#define GL_MAX_ELEMENTS_VERTICES                0x80E8
#define GL_MAX_ELEMENTS_INDICES                 0x80E9
#define GL_BGR                                  0x80E0
#define GL_BGRA                                 0x80E1
#define GL_UNSIGNED_BYTE_3_3_2                  0x8032
#define GL_UNSIGNED_BYTE_2_3_3_REV              0x8362
#define GL_UNSIGNED_SHORT_5_6_5                 0x8363
#define GL_UNSIGNED_SHORT_5_6_5_REV             0x8364
#define GL_UNSIGNED_SHORT_4_4_4_4               0x8033
#define GL_UNSIGNED_SHORT_4_4_4_4_REV           0x8365
#define GL_UNSIGNED_SHORT_5_5_5_1               0x8034
#define GL_UNSIGNED_SHORT_1_5_5_5_REV           0x8366
#define GL_UNSIGNED_INT_8_8_8_8                 0x8035
#define GL_UNSIGNED_INT_8_8_8_8_REV             0x8367
#define GL_UNSIGNED_INT_10_10_10_2              0x8036
#define GL_UNSIGNED_INT_2_10_10_10_REV          0x8368
#define GL_LIGHT_MODEL_COLOR_CONTROL            0x81F8
#define GL_SINGLE_COLOR                         0x81F9
#define GL_SEPARATE_SPECULAR_COLOR              0x81FA
#define GL_TEXTURE_MIN_LOD                      0x813A
#define GL_TEXTURE_MAX_LOD                      0x813B
#define GL_TEXTURE_BASE_LEVEL                   0x813C
#define GL_TEXTURE_MAX_LEVEL                    0x813D
#define GL_SMOOTH_POINT_SIZE_RANGE              0x0B12
#define GL_SMOOTH_POINT_SIZE_GRANULARITY        0x0B13
#define GL_SMOOTH_LINE_WIDTH_RANGE              0x0B22
#define GL_SMOOTH_LINE_WIDTH_GRANULARITY        0x0B23
#define GL_ALIASED_POINT_SIZE_RANGE             0x846D
#define GL_ALIASED_LINE_WIDTH_RANGE             0x846E
#define GL_PACK_SKIP_IMAGES                     0x806B
#define GL_PACK_IMAGE_HEIGHT                    0x806C
#define GL_UNPACK_SKIP_IMAGES                   0x806D
#define GL_UNPACK_IMAGE_HEIGHT                  0x806E
#define GL_TEXTURE_3D                           0x806F
#define GL_PROXY_TEXTURE_3D                     0x8070
#define GL_TEXTURE_DEPTH                        0x8071
#define GL_TEXTURE_WRAP_R                       0x8072
#define GL_MAX_3D_TEXTURE_SIZE                  0x8073
#define GL_TEXTURE_BINDING_3D                   0x806A

#define GL_CLAMP_TO_EDGE                        0x812F


using namespace Renderer;

//Immediate::Immediate(const Common::DisplayMode &displayMode, const Common::DisplayMode &screen) : mode(Mode::Unknown), displayMode(displayMode), screen(screen) {
Immediate::Immediate(const Common::DisplayMode &displayMode) : mode(Mode::Unknown), displayMode(displayMode) {
    switch (displayMode.Ratio()) {
        case Common::AspectRatio::_4x3:
            virtualHeight = 240;
            verticalOffset = 30;
            break;
        case Common::AspectRatio::_16x9:
            virtualHeight = 180;
            verticalOffset = 0;
            break;
        case Common::AspectRatio::_16x10:
            virtualHeight = 200;
            verticalOffset = 10;
            break;
        default:
            break;
    }

    enableInterfaceMode();
}

void Immediate::enableInterfaceMode() {
    if (mode == Mode::Interface)
        return;

    mode = Mode::Interface;

    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);

    glViewport(0, 0, displayMode.Width(), displayMode.Height());   //This sets up the viewport so that the coordinates (0, 0) are at the top left of the window
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(0, 320, virtualHeight, 0, -10, 10);

    //Back to the modelview so we can draw stuff
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Immediate::drawString(const uint16_t x, const uint16_t y, const uint16_t w, const uint16_t h, const std::string &str, const Common::Colour &colour) {
    enableInterfaceMode();
    glColor4ub(colour.R(), colour.G(), colour.B(), colour.A());
    font.drawString(x, y, w, h, str);
}

void Immediate::drawRect(const uint16_t x, const uint16_t y, const uint16_t w, const uint16_t h, const Common::Colour &colour) {
    enableInterfaceMode();

    glDisable(GL_TEXTURE_2D);
    glColor4ub(colour.R(), colour.G(), colour.B(), colour.A());

    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex2f(x, y);
        glTexCoord2f(1, 0); glVertex2f(x + w, y);
        glTexCoord2f(1, 1); glVertex2f(x + w, y + h);
        glTexCoord2f(0, 1); glVertex2f(x, y + h);
    glEnd();
}

void Immediate::drawQuad(const Vec2d &a, const Vec2d &b, const Vec2d &c, const Vec2d &d, const Common::Colour &colour) {
    enableInterfaceMode();

    glDisable(GL_TEXTURE_2D);
    glColor4ub(colour.R(), colour.G(), colour.B(), colour.A());

    glBegin(GL_QUADS);
        glVertex2f(a.X(), a.Y());
        glVertex2f(b.X(), b.Y());
        glVertex2f(c.X(), c.Y());
        glVertex2f(d.X(), d.Y());
    glEnd();
}

void Immediate::drawLine(const Vec2d &start, const Vec2d &end, const Common::Colour &colour) {
    enableInterfaceMode();

    glDisable(GL_TEXTURE_2D);
    glColor4ub(colour.R(), colour.G(), colour.B(), colour.A());
    glBegin(GL_LINES);
        glVertex2f(start.X(), start.Y());
        glVertex2f(end.X(), end.Y());
    glEnd();
}

void Immediate::drawPoint(const uint16_t x, const uint16_t y, const Common::Colour &colour, const uint16_t size) {
    enableInterfaceMode();

    glPointSize(size);
    glDisable(GL_TEXTURE_2D);
    glColor4ub(colour.R(), colour.G(), colour.B(), 255);
    glBegin(GL_POINTS);
        glVertex2i(x, y);
    glEnd();
}

void Immediate::drawBuffer(const uint32_t *buffer, uint32_t width, uint32_t height, uint32_t size) {
    enableInterfaceMode();

    glEnable(GL_BLEND);

    GLuint screen;
    glGenTextures(1, &screen);

    glBindTexture(GL_TEXTURE_2D, screen);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8, buffer);
    glEnable(GL_TEXTURE_2D);

    int toffset = verticalOffset;
    int boffset = verticalOffset;
    int loffset = 0;
    int roffset = 0;

    int VWIDTH = 640;
    int VHEIGHT = virtualHeight;

    glBegin(GL_QUADS);
        glTexCoord2i(0, 0);
        glVertex3i(loffset, toffset, 0);

        glTexCoord2i(1, 0);
        glVertex3i(VWIDTH*size-roffset, toffset, 0);

        glTexCoord2i(1, 1);
        glVertex3i(VWIDTH*size-roffset, (VHEIGHT-boffset)*size, 0);

        glTexCoord2i(0, 1);
        glVertex3i(loffset, (VHEIGHT-boffset)*size, 0);
    glEnd();

    glDeleteTextures(1, &screen);
}

void Immediate::changeDisplayMode(const Common::DisplayMode &displayMode) {
    switch (displayMode.Ratio()) {
        case Common::AspectRatio::_4x3:
            virtualHeight = 480;
            verticalOffset = 60;
            break;
        case Common::AspectRatio::_16x9:
            virtualHeight = 360;
            verticalOffset = 0;
            break;
        case Common::AspectRatio::_16x10:
            virtualHeight = 400;
            verticalOffset = 20;
            break;
        default:
            break;
    }

    this->displayMode = displayMode;
    mode = Mode::Unknown;
}

Immediate::~Immediate() {

}
