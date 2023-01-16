FROM ubuntu:20.04
ENV DEBIAN_FRONTEND=noninteractive

# Install toolchain
RUN apt-get -y update && apt-get -y install \
    git build-essential

# Clone the repo
RUN git clone https://github.com/cesanta/mongoose.git --depth=1

# Build the web server
RUN cd mongoose/examples/http-server && make server PROG=server
