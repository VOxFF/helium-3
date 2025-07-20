#include "interfaces/ILog.h"

#include <memory>
#include <grpcpp/grpcpp.h>
#include <google/protobuf/empty.pb.h>

#include "event.pb.h"
#include "event.grpc.pb.h"


using namespace helium3::logging;


namespace Helium3 {

class grpcLog : public ILog {
public:
    virtual ~grpcLog() = default;

    grpcLog(std::shared_ptr<grpc::Channel> ch) : m_stub(Logger::NewStub(ch)) {}

    // --- From ILog ---
    void add(const Event& e, TaskState ts) override;
    const Events& events(TaskState taskState = TaskState::Complete) const override;

    std::vector<std::string> loggedIds() const override;
    const MachineSummary& summary(const std::string& machineId) const override;

private:
    std::unique_ptr<Logger::Stub> m_stub;

};

}   //end of namespace Helium3
