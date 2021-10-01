#include "Sys/SFML.h"

#include <algorithm>

#include <iostream>

#include <SFML/Audio.hpp>

static void clientKeyDown(Client::State &clientState, const sf::Event::KeyEvent &sfEvent) {
    Client::KeyPress event;

    event.keyCode = sfEvent.code;
    event.shiftMod = sfEvent.shift;
    event.ctrlMod = sfEvent.control;
    event.altMod = sfEvent.alt;
    event.guiMod = sfEvent.system;

    clientState.keyDown(event);
}

static void clientKeyUp(Client::State &clientState, const sf::Event::KeyEvent &sfEvent) {
    Client::KeyPress event;

    event.keyCode = sfEvent.code;
    event.shiftMod = sfEvent.shift;
    event.ctrlMod = sfEvent.control;
    event.altMod = sfEvent.alt;
    event.guiMod = sfEvent.system;

    clientState.keyUp(event);
}

static void clientMouseMove(Client::State &clientState, const sf::Event::MouseMoveEvent &sfEvent) {
    Client::MouseMove event;

    event.x = sfEvent.x;
    event.y = sfEvent.y;
    event.xrel = sfEvent.x;
    event.yrel = sfEvent.y;

    clientState.mouseMove(event);
}

static void clientMouseButtonPress(Client::State &clientState, const sf::Event::MouseButtonEvent &sfEvent) {
    Client::MouseClick event;

    event.x = sfEvent.x;
    event.y = sfEvent.y;
    event.leftPressed = sfEvent.button == sf::Mouse::Left;
    event.middlePressed = sfEvent.button == sf::Mouse::Middle;
    event.rightPressed = sfEvent.button == sf::Mouse::Right;
    event.x1Pressed = sfEvent.button == sf::Mouse::XButton1;
    event.x2Pressed = sfEvent.button == sf::Mouse::XButton2;

    clientState.mouseButtonPress(event);
}

static void clientMouseButtonRelease(Client::State &clientState, const sf::Event::MouseButtonEvent &sfEvent) {
    Client::MouseClick event;

    event.x = sfEvent.x;
    event.y = sfEvent.y;
    event.leftPressed = sfEvent.button == sf::Mouse::Left;
    event.middlePressed = sfEvent.button == sf::Mouse::Middle;
    event.rightPressed = sfEvent.button == sf::Mouse::Right;
    event.x1Pressed = sfEvent.button == sf::Mouse::XButton1;
    event.x2Pressed = sfEvent.button == sf::Mouse::XButton2;

    clientState.mouseButtonRelease(event);
}

Common::DisplayMode Sys::SFML::changeDisplayMode(const Common::DisplayMode &displayMode, bool fullscreen) {
    uint32_t style = sf::Style::Default;

    if (currentDisplayMode() == displayMode && fullscreen == isFullScreen())
        return displayMode;

    if (fullscreen) { 
        style = sf::Style::Fullscreen;
    }

    auto settings = window.getSettings();

    window.setActive(false);
    window.create(sf::VideoMode(displayMode.Width(), displayMode.Height()), title, style, settings);
    window.setActive(true);

    isFullscreen = fullscreen;

    return displayMode;
}

std::vector<Common::DisplayMode> Sys::SFML::getDisplayModes() const {
    std::vector<Common::DisplayMode> modes;

    for (auto mode : sf::VideoMode::getFullscreenModes()) {
        Common::DisplayMode displayMode(mode.width, mode.height, mode.bitsPerPixel);

        if (displayMode.Ratio() != Common::AspectRatio::Ignore) {
            modes.push_back(displayMode);
        }
    }

    if (modes.front().Width() > modes.back().Width()) {
        std::reverse(modes.begin(), modes.end());
    }
    return modes;
}

Common::DisplayMode Sys::SFML::findDisplayMode(uint16_t width, uint16_t height) const {
    auto modeList = getDisplayModes();

    for (auto mode : modeList) {
        if (mode.Width() == width && mode.Height() == height) {
            return mode;
        }
    }

    return modeList[0];
}

Common::DisplayMode Sys::SFML::currentDisplayMode() const {
    auto size = window.getSize();
    return Sys::SFML::findDisplayMode(size.x, size.y);
}

std::pair<Common::DisplayMode, Common::DisplayMode> Sys::SFML::getPreviousNextMode(const Common::DisplayMode &displayMode) const {
    auto modeList = getDisplayModes();
    Common::DisplayMode previous = displayMode;
    Common::DisplayMode next = displayMode;

    size_t i = 0;
    for (auto mode : modeList) {
        if (mode == displayMode)
            break;

        i++;
    }

    if (i > 0)
        previous = modeList[i-1];
    if (i != modeList.size())
        next = modeList[i+1];
    return std::pair<Common::DisplayMode, Common::DisplayMode>(previous, next);
}

bool Sys::SFML::isFullScreen() const {
    return isFullscreen;
}

bool Sys::SFML::handleEvents(Client::State &clientState) {
    sf::Event event;
    bool run = true;

    while (window.pollEvent(event)) {
        switch (event.type) {
            case sf::Event::KeyPressed:
                clientKeyDown(clientState, event.key);
                break;
            case sf::Event::KeyReleased:
                clientKeyUp(clientState, event.key);
                break;
            case sf::Event::MouseButtonPressed:
                clientMouseButtonPress(clientState, event.mouseButton);
                break;
            case sf::Event::MouseButtonReleased:
                clientMouseButtonRelease(clientState, event.mouseButton);
                break;
            case sf::Event::MouseMoved:
                clientMouseMove(clientState, event.mouseMove);
                break;
            case sf::Event::Closed:
                run = false;
                break;
            default:
                break;
        }
    }

    return run;
}

#if 0
class ToneStream : public sf::SoundStream {
        Audio::Tone tone;
        std::vector<int16_t> m_samples;
        std::size_t m_currentSample;

    public:
        ToneStream(Audio::Tone tone) : tone(tone) {

        }

        void SetBufSize(int sz, int numChannel, int sampleRate) {
            m_samples.resize(sz, 0);
            m_currentSample = 0;
            initialize(numChannel, sampleRate);
        }

        virtual bool onGetData(Chunk& data) {
            data.samples = &m_samples[m_currentSample];
            data.sampleCount = 256;
            m_currentSample = 0;

            tone.generateSamples(&m_samples[m_currentSample], 256);

            return true;
        }

        virtual void onSeek(sf::Time timeOffset) {
            m_currentSample = static_cast<std::size_t>(timeOffset.asSeconds() * getSampleRate() * getChannelCount());
        }

        ~ToneStream() {

        }
};
#endif

static int tonecallback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData) {
    Audio::Tone *voices = (Audio::Tone *)userData;

    float *out = (float *)outputBuffer;

    voices[0].generateSamples(out, framesPerBuffer);

    return 0;
}

void Sys::SFML::sound(float frequency, uint16_t duration, int waveForm) {
    voices[0].tone(frequency, duration, waveForm);
}

Sys::SFML::SFML(const std::string &title) : title(std::string("SFML ") + title), isFullscreen(false) {
    uint32_t style = sf::Style::Default;

    if (isFullscreen) {
        style = sf::Style::Fullscreen;
    }

    auto mode = getDisplayModes().front();

    window.create(sf::VideoMode(mode.Width(), mode.Height()), this->title, style, sf::ContextSettings(32));
    window.setActive(true);

    window.setKeyRepeatEnabled(false);

    clock.restart();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (int i = 0; i < VOICE_COUNT; i++)
        voices[i] = Audio::Tone();

//    ToneStream stream(tone);
//    stream.SetBufSize(256, 1, Audio::FREQUENCY);
//    stream.play();

    if (Pa_Initialize() == paNoError && Pa_OpenDefaultStream(
        &stream,
        0,          // no input channels
        1,          // mono output
        paFloat32,
        Audio::FREQUENCY,
        256,        // frames per buffer
        tonecallback,
        voices.data()
    ) == paNoError && Pa_StartStream(stream) == paNoError);

}

Sys::SFML::~SFML() {
    Pa_CloseStream(stream);
    Pa_Terminate();

    window.setActive(false);
    window.close();
}

