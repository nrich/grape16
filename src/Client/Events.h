#ifndef __CLIENT_EVENTS_H__
#define __CLIENT_EVENTS_H__

#include <cstdint>

namespace Client {
    struct KeyPress {
        uint32_t keyCode; 
        bool shiftMod;
        bool ctrlMod;
        bool altMod;
        bool guiMod;
    };

    struct MouseMove {
        int32_t x;
        int32_t y;
        int32_t xrel;
        int32_t yrel;

/*
        bool leftPressed;
        bool middlePressed;
        bool rightPressed;
        bool x1Pressed;
        bool x2Pressed;
*/
    };

    struct MouseClick {
        int32_t x;
        int32_t y;

        bool leftPressed;
        bool middlePressed;
        bool rightPressed;
        bool x1Pressed;
        bool x2Pressed;
    };
}; // namespace Client

#endif //__CLIENT_EVENTS_H__
