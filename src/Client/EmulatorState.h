#ifndef __CLIENT_EMULATORSTATE_H__
#define __CLIENT_EMULATORSTATE_H__

#include <queue>
#include <optional>

#include "Common/Shared.h"
#include "Math/Point2.h"
#include "Emulator/VM.h"
#include "Emulator/Basic.h"
#include "Client/BaseState.h"

namespace Client {
    class State;

    struct VoiceConfig {
        uint8_t waveForm;
        uint8_t volume;
        uint8_t attack;
        uint8_t decay;
        uint8_t sustain;
        uint8_t release;

        VoiceConfig() : waveForm(0), volume(255), attack(0), decay(0), sustain(255), release(0) {
        }

        VoiceConfig(uint8_t waveForm, uint8_t volume, uint8_t attack, uint8_t decay, uint8_t sustain, uint8_t release) : waveForm(waveForm), volume(volume), attack(attack), decay(decay), sustain(sustain), release(release) {
        }
    };

    struct SoundBufferObject {
        uint8_t voice;
        float frequency;
        uint16_t duration;
        VoiceConfig voiceConfig;

        SoundBufferObject(uint8_t voice, float frequency, uint16_t duration, VoiceConfig voiceConfig) : voice(voice), frequency(frequency), duration(duration), voiceConfig(voiceConfig) {
        }
    };


    class SystemIO : public Emulator::SysIO {
            const static int32_t chars = 40;
            const static int32_t lines = 22;

            Point cursor;

            std::vector<std::array<char, chars>> screenbuffer;
            std::array<uint32_t, 320*180> screen;

            std::queue<char> inputBuffer;
            std::queue<SoundBufferObject> soundBuffer;

            std::array<Common::Colour, 256> colourLookup;
            std::array<VoiceConfig, VOICE_COUNT> voices;
        public:
            SystemIO();

            void cls();

            void write(uint8_t c);

            uint8_t read(bool noecho);

            void puts(const std::string &str);

            std::string gets();

            std::vector<std::array<char, chars>> getScreenBuffer() {
                return screenbuffer;
            }

            void buffer(char c) {
                inputBuffer.push(c);
            }

            char buffer() {
                if (inputBuffer.size() == 0)
                    return 0;

                char c = inputBuffer.front();
                inputBuffer.pop();
                return c;
            }

            void setpixel(uint16_t x, uint16_t y, uint8_t pixel);
            uint8_t getpixel(uint16_t x, uint16_t y);

            void blit(uint16_t x, uint16_t y, std::vector<uint8_t> buffer);

            void sound(uint8_t voice, float frequency, uint16_t duration);
            void voice(uint8_t voice, uint8_t waveForm, uint8_t volume, uint8_t attack, uint8_t decay, uint8_t sustain, uint8_t release);

            std::array<uint32_t, 320*180> getScreen() {
                return screen;
            }

            std::optional<SoundBufferObject> nextSound() {
                if (soundBuffer.size() == 0)
                    return std::nullopt;

                auto sound = soundBuffer.front();
                soundBuffer.pop();
                return sound;
            }

            VoiceConfig getVoice(const uint8_t voice) {
                return voices[voice];
            }
    };

    class EmulatorState : public BaseState {
            std::shared_ptr<Emulator::VM> vm;
            std::shared_ptr<Emulator::Program> program;
            std::stack<char> keypresses;
            uint32_t clockspeed;
            std::shared_ptr<SystemIO> sysio;

            std::map<uint32_t, std::vector<Emulator::BasicToken>> basic;
            const bool debug;
        public:
            EmulatorState(std::shared_ptr<Emulator::VM> vm, std::shared_ptr<Emulator::Program> program, uint32_t clockspeed, bool debug) : vm(vm), program(program), clockspeed(clockspeed), debug(debug) {
                sysio = std::make_shared<SystemIO>();
            }

            void onRender(State *state, const uint32_t time);
            void onTick(State *state, const uint32_t time);
            void onMouseMove(State *state, const MouseMove &event);
            void onMouseButtonPress(State *state, const MouseClick &event);
            void onMouseButtonRelease(State *state, const MouseClick &event);
            void onKeyDown(State *state, const KeyPress &event);
            void onKeyUp(State *state, const KeyPress &event);

            void onEnterState(State *state, std::any data) {
                sysio->cls();
            }

            void onLeaveState(State *state, std::any data) {
                sysio->cls();
            }
    };
};

#endif //__CLIENT_EMULATORSTATE_H__
