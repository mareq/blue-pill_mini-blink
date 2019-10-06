# Minimal STM32 BluePill Project

This project demonstrates the absolute minimum that needs to be done in order to produce working code for STM32 BluePill. It is largely based on examples by [PurpleAlien](https://github.com/PurpleAlien/stm32-minimal) and [geokon-gh](https://github.com/geokon-gh/bluepill), with emphasis on *absolute minimum*, i.e. anything that is not absolutely necessary has been stripped off.

It is the simple mini-blink program that will flash the LED. In order to keep things simple, the mini-blink program is using GPIO pin C13, which already has on-board LED connected to it (note that the LED is connected in such a way that it has negative logic).


## Requirements

All code has been compiled on Debian Buster and tested on [STM32F103C8T6](https://stm32-base.org/boards/STM32F103C8T6-Blue-Pill.html) a.k.a. BluePill.

Apart of the Linux box that is used to compile the code, the following hardware is needed:
* [STM32F103C8T6](https://stm32-base.org/boards/STM32F103C8T6-Blue-Pill.html)
* [ST-Link V2](https://www.ebay.com/itm/1PCS-ST-Link-V2-mini-STM8-STM32-STLINK-simulator-download-programming-With-Cover/183686668313)

Following is the minimal set of packages that need to be installed on the system:
```
# apt update
# apt install build-essential gcc-arm-none-eabi cmake stlink-tools openocd telnet gdb-multiarch
```

Optionally, instead of running commands directly on the Linux box, they can be run inside the Docker container. The [`Dockerfile`](./docker/Dockerfile) with the minimal required configuration is provided as part of this repository. It can be easily built simply by running the [`build-image.sh`](./docker/build-image.sh) script, which will create the Docker image named `blue-pill:mini-blink`.


## Instructions

### Compile

Compile the source code:
```
$ mkdir build && cd build
$ cmake -DCMAKE_TOOLCHAIN_FILE=../toolchain.cmake ..
  -- The C compiler identification is GNU 7.3.1
  -- The CXX compiler identification is GNU 7.3.1
  -- Check for working C compiler: /usr/bin/arm-none-eabi-gcc
  -- Check for working C compiler: /usr/bin/arm-none-eabi-gcc -- works
  -- Detecting C compiler ABI info
  -- Detecting C compiler ABI info - done
  -- Detecting C compile features
  -- Detecting C compile features - done
  -- Check for working CXX compiler: /usr/bin/arm-none-eabi-g++
  -- Check for working CXX compiler: /usr/bin/arm-none-eabi-g++ -- works
  -- Detecting CXX compiler ABI info
  -- Detecting CXX compiler ABI info - done
  -- Detecting CXX compile features
  -- Detecting CXX compile features - done
  -- CMAKE_BUILD_TYPE: <Debug>
  -- The ASM compiler identification is GNU
  -- Found assembler: /usr/bin/arm-none-eabi-gcc
  -- Configuring done
  -- Generating done
  -- Build files have been written to: ..snip../blue-pill_mini-blink/build
$ make
  Scanning dependencies of target mini-blink.elf
  [ 33%] Building C object CMakeFiles/mini-blink.elf.dir/init_stm32f103x8.c.obj
  [ 66%] Building C object CMakeFiles/mini-blink.elf.dir/src/main.c.obj
  [100%] Linking C executable mini-blink.elf
  [100%] Built target mini-blink.elf
  Scanning dependencies of target mini-blink.bin
  [100%] Built target mini-blink.bin
  Scanning dependencies of target mini-blink.hex
  [100%] Built target mini-blink.hex
```

### Flash

Flash the binary using `st-flash`:
```
$ st-flash write mini-blink.bin 0x08000000
  st-flash 1.5.1
  2019-10-06T16:16:46 INFO common.c: Loading device parameters....
  2019-10-06T16:16:46 INFO common.c: Device connected is: F1 Medium-density device, id 0x20036410
  2019-10-06T16:16:46 INFO common.c: SRAM size: 0x5000 bytes (20 KiB), Flash: 0x10000 bytes (64 KiB) in pages of 1024 bytes
  2019-10-06T16:16:46 INFO common.c: Attempting to write 824 (0x338) bytes to stm32 address: 134217728 (0x8000000)
  Flash page at addr: 0x08000000 erased
  2019-10-06T16:16:46 INFO common.c: Finished erasing 1 pages of 1024 (0x400) bytes
  2019-10-06T16:16:46 INFO common.c: Starting Flash write for VL/F0/F3/F1_XL core id
  2019-10-06T16:16:46 INFO flash_loader.c: Successfully loaded flash loader in sram
    1/1 pages written
  2019-10-06T16:16:46 INFO common.c: Starting verification of write complete
  2019-10-06T16:16:46 INFO common.c: Flash written and verified! jolly good!
```
This is one of three possible ways how to flash the binary (see below: `telnet`: `flash write_image` and `gdb`: `load`).

### Debug

Connect with OpenOCD:
```
$ cd stlink
$ openocd -f custom_stlink-v2.cfg
```

Telnet to OpenOCD and flash the binary:
```
$ telnet localhost 4444
  Trying ::1...
  Trying 127.0.0.1...
  Connected to localhost.
  Escape character is '^]'.
  Open On-Chip Debugger
> reset halt
  target halted due to debug-request, current mode: Thread
  xPSR: 0x01000000 pc: 0x08000150 msp: 0x20005000
> flash write_image erase ../build/mini-blink.bin 0x08000000
  auto erase enabled
  device id = 0x20036410
  flash size = 64kbytes
  target halted due to breakpoint, current mode: Thread
  xPSR: 0x61000000 pc: 0x2000003a msp: 0x20005000
  wrote 1024 bytes from file ../build/mini-blink.bin in 0.104287s (9.589 KiB/s)
> reset run
```
The command `flash write_image` will flash the binary onto the BluePill.

Connect to OpenOCD with the GNU debugger:
```
$ gdb-multiarch build/mini-blink.elf
(gdb) target remote localhost:3333
  Remote debugging using localhost:3333
  reset_handler () at /home/mareq/work/tutor/projects/mcu/repo/blue-pill/init_stm32f103x8.c:7
(gdb) monitor reset halt
  target halted due to debug-request, current mode: Thread
  xPSR: 0x01000000 pc: 0x08000150 msp: 0x20005000
(gdb) load
  Loading section .text, size 0x330 lma 0x8000000
  Loading section .init, size 0x4 lma 0x8000330
  Loading section .fini, size 0x4 lma 0x8000334
  Start address 0x8000150, load size 824
  Transfer rate: 6 KB/sec, 274 bytes/write.
(gdb) ..debug..
```
The command `load` will flash the binary onto the BluePill.


## Documentation

The source code itself has been heavily commented, please refer to those comments. There are numerous references from source code comments to the [official STM32 documentation](https://www.st.com/content/ccc/resource/technical/document/reference_manual/59/b9/ba/7f/11/af/43/d5/CD00171190.pdf/files/CD00171190.pdf/jcr:content/translations/en.CD00171190.pdf) for the [STM32F103C8](https://www.st.com/content/st_com/en/products/microcontrollers-microprocessors/stm32-32-bit-arm-cortex-mcus/stm32-mainstream-mcus/stm32f1-series/stm32f103/stm32f103c8.html#resource). This documentation is [included](./documents/stm32f103xx.pdf) in this repository for completeness and convenience.


