FROM ubuntu:focal AS firmware_build

# Install common dependencies
RUN apt update && apt install -y \
    build-essential \
    git \
    python3 \
    python3-pip

# Install Bitcraze dependencies
RUN apt update &&\
    apt install -y make gcc-arm-none-eabi

# Install dependencies for CLOAD
RUN python3 -m pip install cflib cfclient

WORKDIR /base

COPY . .

RUN cd source && mkdir build && echo "{\"tag\": \"105\"}" >> build/build_info.json
RUN cd source && \
    make -j $(nproc)

FROM firmware_build AS load
ENTRYPOINT "$(cd source && make cload)"

FROM firmware_build AS final

ENV REMOTE_COMPILER_PORT=5003

RUN apt update && apt install -y --no-install-recommends build-essential python3 python3.8-venv

WORKDIR /base/remote_compiler
RUN python3 -m venv ./venv
ENV PATH="/base/remote_compiler/venv/bin:$PATH"

RUN python requirements.py

ENTRYPOINT ["python", "app.py", "5003"]

FROM ubuntu:focal AS lint
RUN apt update && apt install -y clang-format

WORKDIR /app/lint
COPY ./source ./

ENTRYPOINT ["bash", "format.sh"]

