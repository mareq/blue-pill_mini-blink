# base image: debian buster (Debian 10.0, released 2019-07-06)
FROM debian:buster

# install the minimal set of needed packages
RUN apt-get update && \
    apt-get --fix-missing --yes install \
      build-essential \
      gcc-arm-none-eabi \
      cmake \
      stlink-tools \
      openocd \
      telnet \
      gdb-multiarch

# clean-up
RUN apt-get autoremove --yes && \
    apt-get autoclean


# vim: set ts=2 sw=2 et:


