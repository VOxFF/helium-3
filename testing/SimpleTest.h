#pragma once

#include "SimulationTest.h"
#include <gtest/gtest.h>

namespace Helium3::Testing {

/**
 * @brief Parameterized test that runs the simulation with random mining durations
 *        and logs all activity without verifying specific values.
 *
 * Uses SimulationTest::run() over various SimulationParams to exercise the
 * event pipeline end-to-end for specied duraton.
 */
class SimpleTest
  : public SimulationTest
  , public ::testing::TestWithParam<SimulationParams>
{
protected:
    void SetUp() override {
        init(GetParam());
    }
};

} // namespace Helium3::Testing
