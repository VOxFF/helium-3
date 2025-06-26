#pragma once

#include "implementation/Simulation.h"
#include "implementation/TrucksManager.h"
#include "common/Time.h"

#include <gtest/gtest.h>
#include <memory>
#include <functional>

namespace Helium3::Testing {

    
struct SimulationParams {
    unsigned int truckCount;
    unsigned int stationCount;
    Duration simulationLength;
    TruckFactory factory;
};

class SimulationTest {
protected:
    void init(const SimulationParams& params) {
        m_simulation = std::make_unique<Simulation>(params.factory);
        m_simulation->initialize(params.truckCount, params.stationCount);
        m_length = params.simulationLength;
    }

    void run() {
        m_simulation->run(m_length);
    }

    std::unique_ptr<ISimulation> m_simulation;
    Duration m_length;
};

/*
This test is used with non-random mining / predicatable mining duratons 
*/
struct PredictedParams {
    SimulationParams simulatonParams;
    std::unordered_map<std::string, ILog::MachineSummary> expectedSummary;
};


class PredictedTest : public SimulationTest, public ::testing::TestWithParam<PredictedParams> {
protected:
    void SetUp() override {
        init(GetParam().simulatonParams);
    }
};

inline void PrintTo(const Helium3::Testing::PredictedParams& params, std::ostream* os) {
    *os << "PredictedParams {\n"
        << "  trucks   = " << params.simulatonParams.truckCount << ",\n"
        << "  stations = " << params.simulatonParams.stationCount << ",\n"
        << "  duration = " 
        << std::chrono::duration_cast<std::chrono::hours>(params.simulatonParams.simulationLength).count() << "h,\n"
        << "  expected = {\n";

    for (const auto& [machineId, summary] : params.expectedSummary) {
        *os << "    " << machineId << " => {\n";
        *os << "      complette = {\n";
        for (const auto& [state, stats] : summary.complette) {
            *os << "        " << state
                << ": (" << stats.occurrences << "x, "
                << std::chrono::duration_cast<std::chrono::minutes>(stats.totalDuration).count() << "m)\n";
        }
        *os << "      },\n";
        *os << "      unfinshed = {\n";
        for (const auto& [state, stats] : summary.unfinshed) {
            *os << "        " << state
                << ": (" << stats.occurrences << "x, "
                << std::chrono::duration_cast<std::chrono::minutes>(stats.totalDuration).count() << "m)\n";
        }
        *os << "      }\n";
        *os << "    },\n";
    }

    *os << "  }\n"
        << "}";
}

} // namespace Helium3::Testing
