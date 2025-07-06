FROM conanio/gcc13-ubuntu16.04:latest

WORKDIR /app

COPY . .
RUN mkdir build

WORKDIR build

RUN conan remote add conancenterv2 https://center2.conan.io
RUN conan install .. --output-folder=. --build=missing
RUN cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
RUN cmake --build .
