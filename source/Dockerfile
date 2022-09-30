FROM ubuntu:focal as build

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

WORKDIR /build

COPY . .

RUN cd source && \
    make -j $(nproc)

ENTRYPOINT "$(cd source && make cload)"
