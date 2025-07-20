#!/usr/bin/env python3
import os
import sys
import time
from concurrent import futures


here = os.path.dirname(__file__)
gen_py = os.path.abspath(os.path.join(here, os.pardir, "proto", "gen-py"))
sys.path.insert(0, gen_py)


import grpc
from google.protobuf import empty_pb2

import event_pb2
import event_pb2_grpc

class LoggerServicer(event_pb2_grpc.LoggerServicer):
    def LogEvent(self, request, context):
        print(f"[Event] machine_id: {request.machine_id}")
        print(f"        state:      {request.state}")
        print(f"        name:       {request.name}")
        print(f"        start_ns:   {request.start_ns}")
        print(f"        duration_ns:{request.duration_ns}")
        print(f"        message:    {request.message}")
        print(f"        complete:   {request.complete}")
        print("-" * 60)
        return empty_pb2.Empty()

def serve(port: int = 50051):
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=10))
    event_pb2_grpc.add_LoggerServicer_to_server(LoggerServicer(), server)
    server.add_insecure_port(f"[::]:{port}")
    server.start()
    print(f"gRPC Logger server listening on port {port}...")
    try:
        while True:
            time.sleep(86400)
    except KeyboardInterrupt:
        print("Shutting down.")
        server.stop(0)

if __name__ == "__main__":
    serve()
