#include "SimulationTest.h"

#include <iostream>
#include <iomanip>  
#include <ctime>    

namespace Helium3::Testing {

namespace {
constexpr int SUMMARY_WIDTH = 40;
const std::string UNFINISHED = " Unfinished ";
}

 void SimulationTest::printEvent(const Event& e, ILog::TaskState state) const 
 {
    std::cout << (state == ILog::TaskState::Unfinished ? "X " : "  ");
    std::cout 
        << std::left
        << std::setw(10) << (e.machineId + ":")             // "Truck_0:"
        << std::setw(14) << ("(" + e.name + ")")            // "(Unloading)"
        << std::setw(8) << toString(e.start) << " -> "      // "19:21:40"
        << std::setw(8) << toString(e.end())  << " = "      // "19:26:40"
        << std::setw(6) << toString(e.duration)  << "  "    // "5m", "30m"
        << e.message + (state == ILog::TaskState::Unfinished ? " -- Unfinished" : "")
        << std::endl;
}

void SimulationTest::printSummary() const 
{
    const auto& logger = m_simulation->log();
    auto ids = logger.loggedIds();


    for (const auto& id : ids) {
        // Machine header
        std::cout 
            << std::endl
            << id << std::endl
            << std::string(SUMMARY_WIDTH, '-') << std::endl
            << std::left
            << std::setw(17) << "State"
            << std::setw(8)  << "Count"
            << std::setw(10) << "Duration" << std::endl
            << std::string(SUMMARY_WIDTH, '-') << std::endl;

        const auto& summary = logger.summary(id);

        // Completed states
        for (const auto& [state, stats] : summary.complete) {
            std::cout 
                << std::left
                << std::setw(17) << state
                << std::setw(8)  << stats.occurrences
                << std::setw(10) << toString(stats.totalDuration)
                << std::endl;
        }

        // Unfinished section header
        if (!summary.unfinished.empty()) {
            int padLeft  = (SUMMARY_WIDTH - (int)UNFINISHED.size()) / 2;
            int padRight = SUMMARY_WIDTH - (int)UNFINISHED.size() - padLeft;
            std::cout 
                << std::string(padLeft, '-') 
                << UNFINISHED 
                << std::string(padRight, '-') 
                << std::endl;

            // Unfinished states
            for (const auto& [state, stats] : summary.unfinished) {
                std::cout 
                    << std::left
                    << std::setw(17) << state
                    << std::setw(8)  << stats.occurrences
                    << std::setw(10) << toString(stats.totalDuration)
                    << std::endl;
            }
        }
    }
}


void SimulationTest::printAnalytics() const 
{
    struct Analytics {
        Duration busyTime{}; 
        size_t   fullCycles  = 0;                
        size_t   incompleteCycles = 0;           
    };
    std::map<std::string, Analytics> analytics;

    // Different rules for evaluating Trucks and Statations
    auto truckProc = [&](const ILog::MachineSummary& summary, Analytics& a) 
    {
        // fullCycles is count of all finished "Unloading" operations
        if(auto it = summary.complete.find("Unloading"); it != summary.complete.end())
            a.fullCycles = it->second.occurrences;

        // busyTime is a sum of all complete operations except “Waiting”
        for (const auto& [state, ss] : summary.complete) 
            a.busyTime += (state != "Waiting") ? ss.totalDuration : Duration{};
               
        // incompleteCycles is max count of my unfinished operations
        for (const auto& [state, ss] : summary.unfinished) 
            a.incompleteCycles = std::max(ss.occurrences, a.incompleteCycles);
    };

    auto stationProc = [&](const ILog::MachineSummary& summary, Analytics& a) 
    {
        // busyTime is total complete “Unloading” time
        if (auto it = summary.complete.find("Unloading"); it != summary.complete.end()){
            a.busyTime = it->second.totalDuration;
            a.fullCycles = it->second.occurrences;
        }
        //We log only "Unloading"
        a.incompleteCycles = summary.unfinished.size();
    };

    // ID prefix → rules 
    std::vector<std::pair<std::string, 
        std::function<void(const ILog::MachineSummary&, Analytics&)>>> procs = {
        {"Truck",   truckProc},
        {"Station", stationProc}
    };

    // Fill in stats
    const auto& log = m_simulation->log();
    for (auto& id : log.loggedIds()) 
    {
        for (auto& [prefix, proc] : procs) {
            if (id.rfind(prefix, 0) == 0) {                // only if id starts with prefix
                proc(log.summary(id), analytics[id]);
                break;                                     // don’t run any other procs on this id
            }
        }
    }

    // Add stations that have no logged events 
    for (auto i = 0; i < m_params.stationCount; i++) 
    {
        std::string stationId = "Station_" + std::to_string(i);
        if (analytics.find(stationId) == analytics.end()) 
            // Station has no logged events, add with zero analytics
            analytics[stationId] = Analytics{};
    }

    // Print all 
    std::cout << std::endl
        << "Total duration: " << toString(m_params.simulationLength) << std::endl
        << "Trucks:         " << m_params.truckCount  << std::endl
        << "Stations:       " << m_params.stationCount << std::endl
        << std::endl;

    // Table header
    std::cout
        << std::setw(12) << "Machine"
        << std::setw(16) << "Busy Time"
        << std::setw(14) << "Utilization"
        << std::setw(14) << "Full Cycles"
        << std::setw(16) << "Unfinished Cycles" << std::endl
        << std::string(80, '-') << std::endl;

    // Print collected for every machine + eval utilization
    double totalSec = std::chrono::duration_cast<std::chrono::seconds>(
                        m_params.simulationLength).count();

    for (const auto& [id, a] : analytics) 
    {
        double busySec  = std::chrono::duration_cast<std::chrono::seconds>(a.busyTime).count();
        double utilization = busySec / totalSec * 100.0;

        std::ostringstream utilstr;
        utilstr << std::fixed << std::setprecision(1) << utilization << "%";

        std::cout
            << std::setw(12) << id
            << std::setw(18) << toString(a.busyTime)
            << std::setw(16) << utilstr.str()
            << std::setw(16) << a.fullCycles
            << std::setw(10) << a.incompleteCycles
            << std::endl;
    }
    std::cout << std::endl;

}

} // namespace Helium3::Testing


