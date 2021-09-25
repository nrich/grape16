#ifdef _WIN32
#include "mingw.thread.h"
#else
#include <thread>
#endif

#include <cmath>

#include "Audio/Tone.h"
#include "Common/Shared.h"

void Audio::Tone::tone(float freq, uint16_t duration) {
    ToneObject to;
    to.freq = (double)freq;
    to.samplesLeft = duration * FREQUENCY / 1000;

    tones.push(to);
}

void Audio::Tone::generateSamples(float *stream, int length) {
    int i = 0;
    while (i < length) {
        if (tones.empty()) {
            while (i < length) {
                stream[i] = 0;
                i++;
            }
            return;
        }
        ToneObject& to = tones.front();

        int samplesToDo = std::min(i + to.samplesLeft, length);
        to.samplesLeft -= samplesToDo - i;

        while (i < samplesToDo) {
            stream[i] = AMPLITUDE * std::sin(v * 2 * M_PI / FREQUENCY);
            i++;
            v += to.freq;
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

