#!/bin/bash
docker build -t rpi-buildroot docker
docker run -ti --rm -v `pwd`/buildroot:/home/builder/buildroot --name rpi-buildroot rpi-buildroot
