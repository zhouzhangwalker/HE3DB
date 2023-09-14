FROM ubuntu:20.04
#install build dependencies
RUN apt-get update
RUN DEBIAN_FRONTEND=noninteractive apt-get install -y build-essential g++-10  apt-utils ca-certificates git cmake libgmp-dev

# Build HE3DB
COPY . /HEDB
RUN mkdir /HEDB/build
WORKDIR /HEDB/build
RUN cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=g++-10 -DSEAL_THROW_ON_TRANSPARENT_CIPHERTEXT=OFF && make -j4