#ifdef _WIN32
#include "mingw.thread.h"
#else
#include <thread>
#endif

#include <cmath>
#include <random>
#include <array>

#include "Audio/Tone.h"
#include "Common/Shared.h"

#define PINK_MAX_RANDOM_ROWS   (30)
#define PINK_RANDOM_BITS       (24)
#define PINK_RANDOM_SHIFT      ((sizeof(int32_t)*8)-PINK_RANDOM_BITS)

class PinkNoise {
    private:
        std::array<int32_t, PINK_MAX_RANDOM_ROWS> rows;
        int32_t runningSum;     // Used to optimize summing of generators
        int32_t index;          // Incremented each sample
        int32_t indexMask;      // Index wrapped by ANDing with this mask
        float scalar;           // Used to scale within range of -1.0 to +1.0
        uint32_t seed;

        uint32_t generateRandomNumber() {
            static uint32_t randSeed = seed;
            randSeed = (randSeed * 196314165) + 907633515;
            return randSeed;
        }
    public:
        PinkNoise(int numRows) {
            numRows = std::min(PINK_MAX_RANDOM_ROWS, numRows);

            seed = std::chrono::system_clock::now().time_since_epoch().count();
            index = 0;
            indexMask = (1<<numRows) - 1;

            // Calculate maximum possible signed random value. Extra 1 for white noise always added.
            int32_t pmax = (numRows + 1) * (1<<(PINK_RANDOM_BITS-1));
            scalar = 1.0f / pmax;

            rows.fill(0);
            runningSum = 0;
        }

        float generate() {
            int32_t newRandom;
            int32_t sum;
            float output;

            // Increment and mask index
            index = (index + 1) & indexMask;

            // If index is zero, don't update any random values.
            if (index != 0) {
                // Determine how many trailing zeros in PinkIndex.
                // This algorithm will hang if n==0 so test first.
                int numZeros = 0;
                int n = index;
                while((n & 1) == 0) {
                    n = n >> 1;
                    numZeros++;
                }
                // Replace the indexed ROWS random value.
                // Subtract and add back to RunningSum instead of adding all the random
                // values together. Only one changes each time.
                runningSum -= rows[numZeros];
                newRandom = ((int32_t)generateRandomNumber()) >> PINK_RANDOM_SHIFT;
                runningSum += newRandom;
                rows[numZeros] = newRandom;
            }

            // Add extra white noise value.
            newRandom = ((int32_t)generateRandomNumber()) >> PINK_RANDOM_SHIFT;
            sum = runningSum + newRandom;
            // Scale to range of -1.0 to 0.9999.
            output = scalar * sum;

            return output;
        }
};

Audio::Tone::Tone() {
    v = 0.0;
}

void Audio::Tone::tone(float freq, uint16_t duration, int waveForm) {
    ToneObject to;
    to.freq = (double)freq;
    to.samplesLeft = duration * FREQUENCY / 1000;
    to.waveForm = waveForm;

    tones.push(to);
}

void Audio::Tone::generateSamples(float *stream, int length, float amplitude) {
    static PinkNoise pinknoise(16);
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
                case Common::WaveForm::NOISE:
                    stream[i] += amplitude * (pinknoise.generate());
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

