FROM ubuntu:24.04

RUN apt-get update \
    && DEBIAN_FRONTEND=noninteractive apt-get install -y \
    git gcc python3 python3-pip cmake cmake-data ninja-build \
    lcov \
    && rm -rf /var/lib/apt/lists/*

# Instructions from https://github.com/catchorg/Catch2/blob/devel/docs/cmake-integration.md#installing-catch2-from-git-repository
RUN mkdir /user_build && cd /user_build \
    && git clone https://github.com/catchorg/Catch2.git \
    && cd Catch2 \
    && cmake -Bbuild -H. -DBUILD_TESTING=OFF \
    && cmake --build build/ --target install

RUN mkdir /source
VOLUME /source


RUN useradd -u 10000 mchapman
USER mchapman
WORKDIR /home/mchapman

ENTRYPOINT ["/bin/bash"]
CMD ["/source/scripts/docker/in_container/build.sh"]