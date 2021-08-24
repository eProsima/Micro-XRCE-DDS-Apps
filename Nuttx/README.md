# Micro-XRCE-DDS + Nuttx + Olimex STM32 E407 Sample app

This folder contains a build system and some extensions to integrate [Micro-XRCE-DDS](https://micro-xrce-dds.readthedocs.io/en/latest/), [Nuttx](https://nuttx.apache.org/) and the [STM32L4 Discovery kit IoT node](https://www.st.com/en/evaluation-tools/b-l475e-iot01a.html) evaluation board.

This repository has been tested in Nuttx 10.0 and Nuttx 10.1.

## Required hardware

This sample app uses the following hardware:

| Item                           |                                                                                                |
| ------------------------------ | ---------------------------------------------------------------------------------------------- |
| STM32L4 Discovery kit IoT node | [Link](https://www.st.com/en/evaluation-tools/b-l475e-iot01a.html)                             |

## Usage

You can clone this repo directly in the `app` folder of your project.

## Example

In order to test a int32_publisher example for [STM32L4 Discovery kit IoT node](https://www.st.com/en/evaluation-tools/b-l475e-iot01a.html):

1. Install all the Nuttx dependencies using the [official documentation](https://nuttx.apache.org/docs/10.0.0/quickstart/install.html)
2. Clone this repo inside `apps` folder:
```bash
git clone https://github.com/eProsima/Micro-XRCE-DDS-Apps /tmp/Micro-XRCE-DDS-Apps
cp -R /tmp/Micro-XRCE-DDS-Apps/Nuttx/* apps/
```
3. Go to `nuttx` folder and configure the support for [STM32L4 Discovery kit IoT node](https://www.st.com/en/evaluation-tools/b-l475e-iot01a.html):
```bash
cd nuttx
./tools/configure.sh -l b-l475e-iot01a:nsh
```
4. Enable Micro XRCE-DDS Client library, UART4 and Serial Termios using `make menuconfig` or:
```bash
kconfig-tweak --enable CONFIG_MICRO_XRCE_CLIENT
kconfig-tweak --enable CONFIG_MICRO_XRCE_CLIENT_DEMO_APP
kconfig-tweak --enable CONFIG_SERIAL_TERMIOS
kconfig-tweak --enable CONFIG_STM32L4_UART4
kconfig-tweak --enable CONFIG_STM32L4_UART4_SERIALDRIVER
kconfig-tweak --enable CONFIG_UART4_SERIALDRIVER
kconfig-tweak --set-val CONFIG_UART4_RXBUFSIZE 256
kconfig-tweak --set-val CONFIG_UART4_TXBUFSIZE 256
kconfig-tweak --set-val CONFIG_UART4_BAUD 115200
kconfig-tweak --set-val CONFIG_UART4_BITS 8
kconfig-tweak --set-val CONFIG_UART4_PARITY 0
kconfig-tweak --set-val CONFIG_UART4_2STOP 0
```
5. Build Nuttx:
```bash
make -j$(nproc)
```
6. Flash the board:
```bash
openocd -f interface/stlink-v2-1.cfg -f target/stm32l4x.cfg -c init -c "reset halt" -c "flash write_image erase nuttx.bin 0x08000000" -c "reset" -c "exit"
```
7. Connect UART4 (the one in the Arduino header D0 and D1) to your Micro XRCE-DDS Agent.
8. You can run the Micro XRCE-DDS example by launching `microxrcedds_demo_app /dev/ttyS1` in NSH console. First argument is the serial port used for client to agent communication:
```
NuttShell (NSH) NuttX-10.1.0-RC1
nsh> microxrcedds_demo_app /dev/ttyS1
Sent: 0
Sent: 1
Sent: 2
```

## Purpose of the Project

This software is not ready for production use. It has neither been developed nor
tested for a specific use case. However, the license conditions of the
applicable Open Source licenses allow you to adapt the software to your needs.
Before using it in a safety relevant setting, make sure that the software
fulfills your requirements and adjust it according to any applicable safety
standards, e.g., ISO 26262.

## Known Issues/Limitations

There are no known limitations.
