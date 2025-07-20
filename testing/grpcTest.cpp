#include "grpcTest.h"
#include <chrono>

using namespace Helium3::Testing;


INSTANTIATE_TEST_SUITE_P(
    ExtendedScenarios,
    grpcTest,
    ::testing::Values(
        SimulationParams{10,1, std::chrono::hours(3), {}}
    )
);

// Only check “runs” and log event via gRPC
TEST_P(grpcTest, RunsAndLogsOnly) {
    ASSERT_NO_THROW(run());
}
