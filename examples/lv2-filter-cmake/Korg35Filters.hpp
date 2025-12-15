/*
 * Korg 35 24dB Low + High Pass Filters
 * Copyright (C) 2019 Eric Tarr
 * Copyright (C) 2020 Christopher Arndt <info@chrisarndt.de>
 * Copyright (C) 2022 Filipe Coelho <falktx@falktx.com>
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <algorithm>
#include <cstdint>

//========================================================================================
// The following filters are virtual analog models of the Korg 35 low-pass
// filter and high-pass filter found in the MS-10 and MS-20 synthesizers.
// The virtual analog models for the LPF and HPF are different, making these
// filters more interesting than simply tapping different states of the same
// circuit.
//
// These filters were implemented in Faust by Eric Tarr during the
// [2019 Embedded DSP With Faust Workshop](https://ccrma.stanford.edu/workshops/faust-embedded-19/).
//
// Modified by Christopher Arndt to change the cutoff frequency param
// to be given in Hertz instead of normalized 0.0 - 1.0.
//
// #### Filter history:
//
// <https://secretlifeofsynthesizers.com/the-korg-35-filter/>
//========================================================================================

class Korg35Filter
{
    static constexpr const float q = 0.06305821314233212f;

    float c1;
    float c2;
    float c3;
    float r1[2];
    float r2[2];
    float r3[2];
    float r4[2];
    float freq;
    float origfreq;

public:
    Korg35Filter(const float sampleRate = 48000.f)
    {
        origfreq = 0.f;
        setSampleRate(sampleRate);
    }

    void reset()
    {
        r1[0] = r1[1] = 0.f;
        r2[0] = r2[1] = 0.f;
        r3[0] = r3[1] = 0.f;
        r4[0] = r4[1] = 0.f;
    }

    void setFrequency(const float frequency)
    {
        origfreq = frequency;
        freq = c2 * frequency;
    }

    void setSampleRate(const float newSampleRate)
    {
        const float c0 = std::min<float>(192000.f, std::max<float>(1.f, newSampleRate));
        c1 = 3.14159274f / c0;
        c2 = 44.0999985f / c0;
        c3 = 1.0f - c2;
        freq = c2 * origfreq;
        reset();
    }

    inline void processLowPass(const float* const input, float* const output, const uint32_t frames)
    {
        const float f = freq;

        for (uint32_t i = 0; i < frames; ++i)
        {
            r4[0] = f + c3 * r4[1];

            const float t1 = std::tan(c1 * r4[0]);
            const float t2 = (float(input[i]) - r3[1]) * t1;
            const float f3 = t1 + 1.0f;
            const float t4 = 1.0f - t1 / f3;
            const float t5 = (t1
                           * ((r3[1] + (t2 + q * r1[1] * t4) / f3 + r2[1] * (0.f - 1.f / f3)) / (1.f - q * (t1 * t4) / f3) - r1[1])
                           ) / f3;
            const float t6 = r1[1] + t5;

            r1[0] = r1[1] + 2.f * t5;
            r2[0] = r2[1] + 2.f * (t1 * (q * t6 - r2[1])) / f3;
            r3[0] = r3[1] + 2.f * t2 / f3;
            r4[1] = r4[0];
            r1[1] = r1[0];
            r2[1] = r2[0];
            r3[1] = r3[0];

            output[i] = t6;
        }
    }

    inline void processHighPass(const float* const input, float* const output, const uint32_t frames)
    {
        const float f = freq;

        for (uint32_t i = 0; i < frames; ++i)
        {
            r4[0] = f + c3 * r4[1];

            const float t1 = std::tan(c1 * r4[0]);
            const float t2 = (input[i] - r3[1]) * t1;
            const float t3 = t1 + 1.0f;
            const float t4 = t1 / t3;
            const float t5 = (input[i] - (r3[1] + (t2 - r1[1] - r2[1] * (0.f - t4)) / t3)) / (1.f - q * (t1 * (1.f - t4)) / t3);
            const float t6 = q * t5;
            const float t7 = (t1 * (t6 - r2[1])) / t3;

            r1[0] = r1[1] + 2.f * (t1 * (t6 - (t7 + r1[1] + r2[1]))) / t3;
            r2[0] = r2[1] + 2.f * t7;
            r3[0] = r3[1] + 2.f * t2 / t3;
            r4[1] = r4[0];
            r1[1] = r1[0];
            r2[1] = r2[0];
            r3[1] = r3[0];

            output[i] = t5;
        }
    }
};
