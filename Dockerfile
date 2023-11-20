FROM ubuntu:latest

# install build tools
RUN apt-get update

RUN apt-get install -y \
    wget \
    ninja-build \
    git \
    libssl-dev \ 
    gnupg \
    curl \
    zip

RUN apt-get install -y \
    zlib1g-dev \
    libxcomposite1 \
    libxdamage1 \
    libxrandr2 \
    libgbm1 \
    pulseaudio \
    libxfixes-dev
    
# install clang
RUN wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add -

RUN apt-get install -y software-properties-common && \
    add-apt-repository "deb http://apt.llvm.org/focal/ llvm-toolchain-focal main" && \
    apt-get update && \
    apt-get install -y clang-18 \
    libc++-18-dev \
    libc++abi-18-dev

# RUN rm -rf /var/lib/apt/lists/*

RUN update-alternatives --install /usr/bin/clang clang /usr/bin/clang-18 100
RUN update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-18 100


ENV CC=/usr/bin/clang
ENV CXX=/usr/bin/clang++

# install cmake
RUN wget https://github.com/Kitware/CMake/releases/download/v3.27.7/cmake-3.27.7-linux-x86_64.sh && \
    chmod +x cmake-3.27.7-linux-x86_64.sh && \
    ./cmake-3.27.7-linux-x86_64.sh --skip-license --prefix=/usr/local && \
    rm cmake-3.27.7-linux-x86_64.sh

CMD ["bash"]
