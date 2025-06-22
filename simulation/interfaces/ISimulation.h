#pragma once


namespace Helium3 {

class Time;

class ISimulation {

    virtual void initalize(unsigned int trackCount, unsigned int stationCount) = 0;
    virtual void run(const Time& totalTime, const Time& timeStep) = 0;

};

}