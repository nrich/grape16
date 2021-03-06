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


static bool isPowerOfTwo(int x) {
    return (x != 0) && ((x & (x - 1)) == 0);
}

Immediate::Immediate(const Common::DisplayMode &displayMode, Common::AspectRatio ratio, int resolutionScale) : mode(Mode::Unknown), displayMode(displayMode), ratio(ratio), resolutionScale(resolutionScale) {
    if (!isPowerOfTwo(resolutionScale)) {
        std::cerr << "Invalid resolution scale" << std::endl;
        exit(-1);
    }

    configureVirtualDisplay();
    enableInterfaceMode();
}

void Immediate::configureVirtualDisplay() {
    if (ratio == Common::AspectRatio::_4x3) {
        switch (displayMode.Ratio()) {
            case Common::AspectRatio::_4x3:
                virtualWidth = 320;
                horizontalOffset = 0;
                virtualHeight = 240;
                verticalOffset = 0;
                break;
            case Common::AspectRatio::_16x9:
                virtualWidth = 320;
                horizontalOffset = 40;
                virtualHeight = 180;
                verticalOffset = 0;
                break;
            case Common::AspectRatio::_16x10:
                virtualWidth = 320;
                horizontalOffset = 28;
                virtualHeight = 200;
                verticalOffset = 0;
                break;
            default:
                std::cerr << "Invalid aspect ratio" << std::endl;
                exit(-1);
        }

        horizontalRatio = (float)(virtualWidth-2*horizontalOffset)/320.0f;
        verticalRatio = (float)(virtualHeight-2*verticalOffset)/240.0f;
    } else if (ratio == Common::AspectRatio::_16x9) {
        switch (displayMode.Ratio()) {
            case Common::AspectRatio::_4x3:
                virtualWidth = 320;
                horizontalOffset = 0;
                virtualHeight = 240;
                verticalOffset = 30;
                break;
            case Common::AspectRatio::_16x9:
                virtualWidth = 320;
                horizontalOffset = 0;
                virtualHeight = 180;
                verticalOffset = 0;
                break;
            case Common::AspectRatio::_16x10:
                virtualWidth = 320;
                horizontalOffset = 0;
                virtualHeight = 200;
                verticalOffset = 10;
                break;
            default:
                std::cerr << "Invalid aspect ratio" << std::endl;
                exit(-1);
        }

        horizontalRatio = (float)(virtualWidth-2*horizontalOffset)/320.0f;
        verticalRatio = (float)(virtualHeight-2*verticalOffset)/180.0f;
    } else if (ratio == Common::AspectRatio::_16x10) {
        switch (displayMode.Ratio()) {
            case Common::AspectRatio::_4x3:
                virtualWidth = 320;
                horizontalOffset = 0;
                virtualHeight = 240;
                verticalOffset = 20;
                break;
            case Common::AspectRatio::_16x9:
                virtualWidth = 320;
                horizontalOffset = 16;
                virtualHeight = 180;
                verticalOffset = 0;
                break;
            case Common::AspectRatio::_16x10:
                virtualWidth = 320;
                horizontalOffset = 0;
                virtualHeight = 200;
                verticalOffset = 0;
                break;
            default:
                std::cerr << "Invalid aspect ratio" << std::endl;
                exit(-1);
        }

        horizontalRatio = (float)(virtualWidth-2*horizontalOffset)/320.0f;
        verticalRatio = (float)(virtualHeight-2*verticalOffset)/200.0f;
    } else {
        std::cerr << "Invalid aspect ratio" << std::endl;
        exit(-1);
    }

    horizontalRatio *= resolutionScale;
    verticalRatio *= resolutionScale;
    interfaceScale = 2.0f * resolutionScale;
    virtualWidth *= resolutionScale;
    horizontalOffset *= resolutionScale;
    virtualHeight *= resolutionScale;
    verticalOffset *= resolutionScale;
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

    glOrtho(0, virtualWidth*interfaceScale, virtualHeight*interfaceScale, 0, -1, 1);

    //Back to the modelview so we can draw stuff
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Immediate::enableDrawMode() {
    if (mode == Mode::Draw)
        return;

    mode = Mode::Draw;

    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);

    glViewport(0, 0, displayMode.Width(), displayMode.Height());   //This sets up the viewport so that the coordinates (0, 0) are at the top left of the window
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(0, virtualWidth, virtualHeight, 0, -1, 1);

    //Back to the modelview so we can draw stuff
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Immediate::drawString(const uint16_t x, const uint16_t y, const uint16_t w, const uint16_t h, const std::string &str, const Common::Colour &colour) {
    enableInterfaceMode();

    glColor4ub(colour.R(), colour.G(), colour.B(), colour.A());
    font.drawString(((horizontalRatio*x)+horizontalOffset)*interfaceScale, ((verticalRatio*y)+verticalOffset)*interfaceScale, (w*horizontalRatio)*interfaceScale, (h*verticalRatio)*interfaceScale, str);
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
    enableDrawMode();

    glEnable(GL_BLEND);

    GLuint screen;
    glGenTextures(1, &screen);

    glBindTexture(GL_TEXTURE_2D, screen);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, buffer);
    glEnable(GL_TEXTURE_2D);

    int toffset = verticalOffset;
    int boffset = verticalOffset;
    int loffset = horizontalOffset;
    int roffset = horizontalOffset;

    int VWIDTH = virtualWidth;
    int VHEIGHT = virtualHeight;

    auto colour = Common::Colour::White;
    glColor4ub(colour.R(), colour.G(), colour.B(), colour.A());

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

    if (horizontalOffset) {
        glDisable(GL_BLEND);
        auto colour = Common::Colour::DarkBlue;
        glColor4ub(colour.R(), colour.G(), colour.B(), colour.A());

        glDisable(GL_TEXTURE_2D);

        glBegin(GL_QUADS);
            glVertex3i(0, 0, 0);
            glVertex3i(loffset, 0, 0);
            glVertex3i(loffset, VHEIGHT, 0);
            glVertex3i(0, VHEIGHT, 0);
        glEnd();

        glBegin(GL_QUADS);
            glVertex3i(VWIDTH-roffset, 0, 0);
            glVertex3i(VWIDTH, 0, 0);
            glVertex3i(VWIDTH, VHEIGHT, 0);
            glVertex3i(VWIDTH-roffset, VHEIGHT, 0);
        glEnd();
    }

    if (verticalOffset) {
        glDisable(GL_BLEND);
        auto colour = Common::Colour::DarkBlue;
        glColor4ub(colour.R(), colour.G(), colour.B(), colour.A());

        glDisable(GL_TEXTURE_2D);

        glBegin(GL_QUADS);
            glVertex3i(0, 0, 0);
            glVertex3i(VWIDTH, 0, 0);
            glVertex3i(VWIDTH, toffset, 0);
            glVertex3i(0, toffset, 0);
        glEnd();

        glBegin(GL_QUADS);
            glVertex3i(0, VHEIGHT-boffset, 0);
            glVertex3i(VWIDTH, VHEIGHT-boffset, 0);
            glVertex3i(VWIDTH, VHEIGHT, 0);
            glVertex3i(0, VHEIGHT, 0);
        glEnd();
    }

    glDeleteTextures(1, &screen);
}

void Immediate::drawBuffer(const uint8_t *buffer, uint32_t width, uint32_t height, uint32_t size) {
    enableDrawMode();

    glEnable(GL_BLEND);

    GLuint screen;
    glGenTextures(1, &screen);

    glBindTexture(GL_TEXTURE_2D, screen);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R3_G3_B2, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE_3_3_2, buffer);
    glEnable(GL_TEXTURE_2D);

    int toffset = verticalOffset;
    int boffset = verticalOffset;
    int loffset = horizontalOffset;
    int roffset = horizontalOffset;

    int VWIDTH = virtualHeight;
    int VHEIGHT = virtualHeight;

    auto colour = Common::Colour::White;
    glColor4ub(colour.R(), colour.G(), colour.B(), colour.A());

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

    if (horizontalOffset) {
        glDisable(GL_BLEND);
        auto colour = Common::Colour::DarkBlue;
        glColor4ub(colour.R(), colour.G(), colour.B(), colour.A());

        glDisable(GL_TEXTURE_2D);

        glBegin(GL_QUADS);
            glVertex3i(0, 0, 0);
            glVertex3i(loffset, 0, 0);
            glVertex3i(loffset, VHEIGHT, 0);
            glVertex3i(0, VHEIGHT, 0);
        glEnd();

        glBegin(GL_QUADS);
            glVertex3i(VWIDTH-roffset, 0, 0);
            glVertex3i(VWIDTH, 0, 0);
            glVertex3i(VWIDTH, VHEIGHT, 0);
            glVertex3i(VWIDTH-roffset, VHEIGHT, 0);
        glEnd();
    }

    if (verticalOffset) {
        glDisable(GL_BLEND);
        auto colour = Common::Colour::DarkBlue;
        glColor4ub(colour.R(), colour.G(), colour.B(), colour.A());

        glDisable(GL_TEXTURE_2D);

        glBegin(GL_QUADS);
            glVertex3i(0, 0, 0);
            glVertex3i(VWIDTH, 0, 0);
            glVertex3i(VWIDTH, toffset, 0);
            glVertex3i(0, toffset, 0);
        glEnd();

        glBegin(GL_QUADS);
            glVertex3i(0, VHEIGHT-boffset, 0);
            glVertex3i(VWIDTH, VHEIGHT-boffset, 0);
            glVertex3i(VWIDTH, VHEIGHT, 0);
            glVertex3i(0, VHEIGHT, 0);
        glEnd();
    }

    glDeleteTextures(1, &screen);
}


void Immediate::changeDisplayMode(const Common::DisplayMode &displayMode) {
    this->displayMode = displayMode;

    configureVirtualDisplay();
    mode = Mode::Unknown;
    enableInterfaceMode();
}

bool Immediate::translatePoint(Point &screen, const Point &real) {
    float hratio = ((float)virtualWidth) / (float)displayMode.Width();
    float vratio = ((float)virtualHeight) / (float)displayMode.Height();

    auto tx = real.X() * hratio;
    auto ty = real.Y() * vratio;

    if (tx < horizontalOffset)
        return false;
    if (tx > (virtualWidth - horizontalOffset))
        return false;
    if (ty < verticalOffset)
        return false;
    if (ty > (virtualHeight - verticalOffset))
        return false;

    screen = Point((tx-horizontalOffset)/horizontalRatio, (ty-verticalOffset)/verticalRatio);

    return true;
}

Immediate::~Immediate() {

}
