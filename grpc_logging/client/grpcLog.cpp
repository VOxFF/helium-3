#include "grpcLog.h"

using namespace helium3::logging;
using namespace google::protobuf;
using namespace grpc;

namespace Helium3 {



namespace {
const Events EMPTY_EVENTS;
const std::vector<std::string> EMPTY_STRINGS;
const ILog::MachineSummary EMPTY_SUMARRY;
}


void grpcLog::add(const Event& e, TaskState ts)
{
    EventMsg msg;
    msg.set_machine_id(e.machineId);
    msg.set_state(int(e.state));
    msg.set_name(e.name);
    msg.set_start_ns(e.start.time_since_epoch().count());
    msg.set_duration_ns(e.duration.count());
    msg.set_message(e.message);
    msg.set_complete(ts==TaskState::Complete);

    ClientContext ctx;
    Empty resp;

    m_stub->LogEvent(&ctx, msg, &resp);   
}

const Events& grpcLog::events(TaskState taskState) const 
{
    return EMPTY_EVENTS; // out this quizz's scope
}

std::vector<std::string> grpcLog::loggedIds() const 
{
    return EMPTY_STRINGS; // out this quizz's scope
}

const ILog::MachineSummary& grpcLog::summary(const std::string& machineId) const 
{
    return EMPTY_SUMARRY; // out this quizz's scope
}
    

}   //end of namespace Helium3