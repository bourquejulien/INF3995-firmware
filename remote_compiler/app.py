import os

import grpc
import logging
import sys
import atexit

from concurrent import futures
from out import compiler_pb2_grpc
from src.controller import CompilerController
from src.env_handler import EnvHandler

MAX_WORKERS = 5

logger = None
env_Handler = None


def setup_dependencies():
    global env_Handler
    env_Handler = EnvHandler(os.path.dirname(os.path.abspath(__file__)))
    atexit.register(env_Handler.close)


def setup_grpc(port, max_workers):
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=max_workers))
    compiler_pb2_grpc.add_CompilerServicer_to_server(CompilerController(env_Handler), server)
    server.add_insecure_port('[::]:' + port)

    return server


def main():
    port = sys.argv[1]
    setup_dependencies()
    server = setup_grpc(port, MAX_WORKERS)
    server.start()
    logger.info("Starting remote compiler on: %s", port)
    server.wait_for_termination()


if __name__ == '__main__':
    logging.basicConfig(level = logging.INFO)
    logger = logging.getLogger("app")
    main()
