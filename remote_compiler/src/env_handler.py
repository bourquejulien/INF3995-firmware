import uuid
import tempfile
import shutil
import os
import subprocess

from threading import Thread, Lock


class EnvHandler:
    _directories: dict
    _env_path: str
    _source_path: str
    _root_path: str
    _mutex: Lock

    def __init__(self, app_path):
        self._directories = {}
        self._root_path = os.path.join(app_path, "..")

        self._source_path = os.path.join(self._root_path, "source")
        self._env_path = tempfile.mkdtemp()
        self._mutex = Lock()

    def close(self):
        shutil.rmtree(self._env_path)

    def add(self):
        env_id = str(uuid.uuid4())
        env_path = os.path.join(self._env_path, env_id)

        shutil.rmtree(os.path.join(self._source_path,
                    "build"), ignore_errors=True)
        shutil.copytree(self._source_path, env_path)

        with self._mutex:
            self._directories[env_id] = env_path

        return env_id

    def remove(self, env_id):
        if env_id not in self._directories:
            raise Exception("Unknown id")

        with self._mutex:
            env_path = self._directories.pop(env_id)

        shutil.rmtree(env_path)

    def edit(self, env_id, path, content):
        path = os.path.normpath(path)

        with self._mutex:
            if env_id not in self._directories:
                raise Exception("Unknown id")
            base_path = os.path.join(self._directories.get(env_id), "src")

        with open(os.path.join(base_path, path), mode='wb') as file:
            file.write(content)

    def get(self, env_id, path):
        path = os.path.normpath(path)

        with self._mutex:
            if env_id not in self._directories:
                base_path = self._source_path
            else:
                base_path = self._directories.get(env_id)

        base_path = os.path.join(base_path, "src")

        with open(os.path.join(base_path, path), mode='r') as file:
            data = file.read()

        return data.encode('utf-8')

    def build(self, env_id, block_size: int):
        with self._mutex:
            if env_id not in self._directories:
                raise Exception("Unknown id")
            base_path = self._directories.get(env_id)

        build_path = os.path.join(self._root_path, env_id)
        shutil.copytree(base_path, build_path)

        result = subprocess.run(
            ["make"], shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, cwd=build_path)

        if result.returncode != 0:
            shutil.rmtree(build_path)
            raise Exception("Build error")

        with open(os.path.join(build_path, "build", "cf2.bin"), mode='rb') as file:
            while data := file.read(block_size):
                yield data

        shutil.rmtree(build_path)
