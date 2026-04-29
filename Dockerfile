FROM --platform=linux/amd64 ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

# Install required dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    ocaml ocamlbuild automake autoconf libtool \
    wget git cmake perl unzip \
    python-is-python3 \
    libssl-dev \
    && rm -rf /var/lib/apt/lists/*

# Create working directory
WORKDIR /root

# Clone SGX SDK
RUN git clone https://github.com/intel/linux-sgx.git

WORKDIR /root/linux-sgx

# Prepare SDK
RUN make preparation

# Build SDK installer
RUN make sdk_install_pkg

# Install SDK
RUN cd linux/installer/bin && \
    bash sgx_linux_x64_sdk_*.bin --prefix /root/sgxsdk

# Set environment variables
ENV SGX_SDK=/root/sgxsdk
ENV PATH=$SGX_SDK/bin:$PATH
ENV LD_LIBRARY_PATH=$SGX_SDK/lib64

# Copy your lab files into container
WORKDIR /root
COPY . /root/sgx_lab

WORKDIR /root/sgx_lab
