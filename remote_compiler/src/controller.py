from out import compiler_pb2_grpc, compiler_pb2
from src.env_handler import EnvHandler

BLOCK_SIZE = int(1e5)

class CompilerController(compiler_pb2_grpc.CompilerServicer):
    _env_handler : EnvHandler


    def __init__(self, env_handler) -> None:
        super().__init__()
        self._env_handler = env_handler

    def StartSession(self, start_request, context):
        env_id = self._env_handler.add()
        return compiler_pb2.CompilerReply(id=env_id)

    def EndSession(self, compiler_request, context):
        self._env_handler.remove(compiler_request.id)
        return compiler_pb2.CompilerReply(id=compiler_request.id)

    def Edit(self, edit_request, context):
        self._env_handler.edit(edit_request.id, edit_request.path, edit_request.file)
        return compiler_pb2.CompilerReply(id=edit_request.id)

    def Build(self, compiler_request, context):
        blocks = self._env_handler.build(compiler_request.id, BLOCK_SIZE)

        for block in blocks:
            yield compiler_pb2.DataBlock(data=block)
