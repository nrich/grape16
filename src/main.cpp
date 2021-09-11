#include <cstdlib>
#include <memory>
#include <functional>
#include <map>

#include <iostream>

#include "Client/State.h"

#include "Renderer/Immediate.h"

#include "Common/DisplayMode.h"

#include "Sys/SDL2.h"
#include "Sys/SFML.h"


int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_EVERYTHING);

    //auto sys = std::make_shared<Sys::SDL2>("Test");
    auto sys = std::make_shared<Sys::SFML>("Test");

    auto renderer = std::make_shared<Renderer::Immediate>(sys->currentDisplayMode());
    Client::State clientState(std::dynamic_pointer_cast<Renderer::Base>(renderer), std::dynamic_pointer_cast<Sys::Base>(sys));

    static uint32_t lastRender = sys->getTicks();
    uint32_t renderTime = sys->getTicks();

    while (sys->handleEvents(clientState)) {
        sys->clearScreen();
        renderTime = sys->getTicks();
        clientState.render(renderTime - lastRender);
        clientState.tick(renderTime - lastRender);
        lastRender = renderTime;
        sys->swapBuffers();
    }

    exit(0);
}
