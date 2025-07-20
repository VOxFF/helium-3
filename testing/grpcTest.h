
#pragma once

#include "SimulationTest.h"
#include "grpc_logging/client/grpcLog.h"

#include <grpcpp/channel.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>
#include <gtest/gtest.h>

namespace Helium3::Testing {

class grpcTest 
  : public SimulationTest
  , public ::testing::TestWithParam<SimulationParams>
{
protected:
    void SetUp() override {
        
        auto p = GetParam();

        // Create a gRPC channel and ILog factory
        auto channel = grpc::CreateChannel( "localhost:50051", grpc::InsecureChannelCredentials());
        auto factory = [channel](){ return std::make_unique<grpcLog>(channel);};

       
        m_simulation = std::make_unique<Simulation>(TruckFactory{},factory);
        m_simulation->initialize(p.truckCount, p.stationCount);
    }
};

} // namespace Helium3::Testing
