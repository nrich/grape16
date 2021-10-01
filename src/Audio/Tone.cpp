#ifdef _WIN32
#include "mingw.thread.h"
#else
#include <thread>
#endif

#include <cmath>

#include "Audio/Tone.h"
#include "Common/Shared.h"

void Audio::Tone::tone(float freq, uint16_t duration, int waveForm) {
    ToneObject to;
    to.freq = (double)freq;
    to.samplesLeft = duration * FREQUENCY / 1000;
    to.waveForm = waveForm;

    tones.push(to);
}

void Audio::Tone::generateSamples(float *stream, int length, float amplitude) {
    int i = 0;
    while (i < length) {
        if (tones.empty()) {
            while (i < length) {
                stream[i] += 0;
                i++;
            }
            return;
        }
        ToneObject& to = tones.front();

        int samplesToDo = std::min(i + to.samplesLeft, length);
        to.samplesLeft -= samplesToDo - i;

        while (i < samplesToDo) {
            float pos = fmod(v/FREQUENCY,1.0);
            v += to.freq;

            switch (to.waveForm) {
                case Common::WaveForm::SAWTOOTH:
                    stream[i] += amplitude * (pos*2 - 1);
                    break;
                case Common::WaveForm::TRIANGLE:
                    stream[i] += amplitude * (1-std::fabs(pos-0.5)*4);
                    break;
                case Common::WaveForm::SQUARE:
                    stream[i] += amplitude * (std::sin(pos*2*M_PI) >= 0 ? 1.0 : -1.0);
                    break;
                default: // SINE
                    stream[i] += amplitude * (std::sin(pos*2*M_PI));
                    break;
            }
            i++;
        }

        if (to.samplesLeft == 0) {
            tones.pop();
        }
    }
}

void Audio::Tone::wait() {
    size_t size;
    do {
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        size = tones.size();
    } while (size > 0);
}

