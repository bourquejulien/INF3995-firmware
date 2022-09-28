FROM ubuntu:focal as build

# Install common dependencies
RUN apt update && apt install -y \
    build-essential \
    git \
    python3

# Install Bitcraze dependencies
RUN apt update &&\
    apt install -y make gcc-arm-none-eabi

WORKDIR /build

COPY . .

RUN cd source && \
    make -j $(nproc)
