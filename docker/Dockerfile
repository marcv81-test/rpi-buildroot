FROM ubuntu:trusty

RUN apt-get -y update && apt-get -y install \
bc \
g++ \
gcc \
git \
libc-dev \
libncurses-dev \
make \
python \
rsync \
unzip \
wget

RUN adduser --disabled-password --uid 1000 --gecos '' builder
USER builder
WORKDIR /home/builder
CMD /bin/bash
ENV TERM xterm
