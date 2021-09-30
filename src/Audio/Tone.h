#ifndef __AUDIO_TONE_H__
#define __AUDIO_TONE_H__

#include <cstdint>
#include <queue>

namespace Audio {
    const int FREQUENCY = 44100;

    struct ToneObject {
        double freq = 0.0;
        int samplesLeft = 0;
    };

    class Tone {
        private:
            double v;
            std::queue<ToneObject> tones;
        public:
            Tone() { v = 0.0; }
            ~Tone() {}
            void tone(float freq, uint16_t duration);
            void generateSamples(float *stream, int length);
            void wait();
    };
}; // Audio

#endif //__AUDIO_TONE_H__
