#include <grpcpp/grpcpp.h>

#include "echo.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using echo::EchoRequest;
using echo::EchoReply;
using echo::Echo;

class EchoClient {
 public:
  EchoClient()
      : stub_(Echo::NewStub(grpc::CreateChannel(
                           "localhost:50051", grpc::InsecureChannelCredentials()))) {}

  // Assembles the client's payload, sends it and presents the response back
  // from the server.
  std::string Shout(const std::string& message) {
    // Data we are sending to the server.
    EchoRequest request;
    request.set_message(message);

    // Container for the data we expect from the server.
    EchoReply reply;

    // Context for the client. It could be used to convey extra information to
    // the server and/or tweak certain RPC behaviors.
    ClientContext context;

    // The actual RPC.
    Status status = stub_->Shout(&context, request, &reply);

    // Act upon its status.
    if (status.ok()) {
      return reply.message();
    } else {
      std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
      return "RPC failed - is echo service running?";
    }
  }

 private:
  std::unique_ptr<Echo::Stub> stub_;
};

