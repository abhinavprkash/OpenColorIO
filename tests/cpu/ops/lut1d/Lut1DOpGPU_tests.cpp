// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenColorIO Project.


#include "ops/lut1d/Lut1DOpGPU.cpp"

#include "testutils/UnitTest.h"

namespace OCIO = OCIO_NAMESPACE;


// This is unit-testing internal helper functions not exposed to the API,
// not high-level GPU processing.

OCIO_ADD_TEST(Lut1DOp, pad_lut_one_dimension)
{
    const unsigned width = 6;

    // Create a channel multi row and smaller than the expected texture size.

    std::vector<float> channel;
    channel.resize((width - 2) * 3);

    // Fill the channel.

    for (unsigned idx = 0; idx<channel.size() / 3; ++idx)
    {
        channel[3*idx] = float(idx);
        channel[3*idx + 1] = float(idx) + 0.1f;
        channel[3*idx + 2] = float(idx) + 0.2f;
    }

    // Pad the texture values.

    std::vector<float> chn;
    OCIO_CHECK_NO_THROW(OCIO::PadLutChannels(width, 1, channel, chn));

    // Check the values.

    const float res[18] = { 0.0f, 0.1f, 0.2f, 1.0f, 1.1f, 1.2f,
                            2.0f, 2.1f, 2.2f, 3.0f, 3.1f, 3.2f,
                            3.0f, 3.1f, 3.2f, 3.0f, 3.1f, 3.2f };
    OCIO_CHECK_EQUAL(chn.size(), 18);
    for (unsigned idx = 0; idx<chn.size(); ++idx)
    {
        OCIO_CHECK_EQUAL(chn[idx], res[idx]);
    }
}

OCIO_ADD_TEST(Lut1DOp, pad_lut_two_dimension_1)
{
    const unsigned width = 4;
    const unsigned height = 3;

    std::vector<float> channel;
    channel.resize((height * width - 4) * 3);

    for (unsigned idx = 0; idx<channel.size() / 3; ++idx)
    {
        channel[3 * idx] = float(idx);
        channel[3 * idx + 1] = float(idx) + 0.1f;
        channel[3 * idx + 2] = float(idx) + 0.2f;
    }

    std::vector<float> chn;
    OCIO_CHECK_NO_THROW(OCIO::PadLutChannels(width, height, channel, chn));

    const float res[] = {
        0.0f, 0.1f, 0.2f, 1.0f, 1.1f, 1.2f, 2.0f, 2.1f, 2.2f, 3.0f, 3.1f, 3.2f,
        3.0f, 3.1f, 3.2f, 4.0f, 4.1f, 4.2f, 5.0f, 5.1f, 5.2f, 6.0f, 6.1f, 6.2f, 
        6.0f, 6.1f, 6.2f, 7.0f, 7.1f, 7.2f, 7.0f, 7.1f, 7.2f, 7.0f, 7.1f, 7.2f };
    OCIO_CHECK_EQUAL(chn.size(), 36);
    for (unsigned idx = 0; idx<chn.size(); ++idx)
    {
        OCIO_CHECK_EQUAL(chn[idx], res[idx]);
    }
}

OCIO_ADD_TEST(Lut1DOp, pad_lut_two_dimension_2)
{
    const unsigned width = 4;
    const unsigned height = 3;

    std::vector<float> channel;
    channel.resize((height * width - 3) * 3);

    for (unsigned idx = 0; idx<channel.size() / 3; ++idx)
    {
        channel[3 * idx] = float(idx);
        channel[3 * idx + 1] = float(idx) + 0.1f;
        channel[3 * idx + 2] = float(idx) + 0.2f;
    }

    // Special case where size%(width-1) = 0
    std::vector<float> chn;
    OCIO_CHECK_NO_THROW(OCIO::PadLutChannels(width, height, channel, chn));

    // Check the values

    const float res[] = {
        0.0f, 0.1f, 0.2f, 1.0f, 1.1f, 1.2f, 2.0f, 2.1f, 2.2f, 3.0f, 3.1f, 3.2f,
        3.0f, 3.1f, 3.2f, 4.0f, 4.1f, 4.2f, 5.0f, 5.1f, 5.2f, 6.0f, 6.1f, 6.2f,
        6.0f, 6.1f, 6.2f, 7.0f, 7.1f, 7.2f, 8.0f, 8.1f, 8.2f, 8.0f, 8.1f, 8.2f };

    OCIO_CHECK_EQUAL(chn.size(), 36);

    for (unsigned idx = 0; idx<chn.size(); ++idx)
    {
        OCIO_CHECK_EQUAL(chn[idx], res[idx]);
    }
}

