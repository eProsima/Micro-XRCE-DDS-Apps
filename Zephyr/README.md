**Work in progress**

# Micro-XRCE-DDS + Zephyr + Olimex STM32 E407 Sample app

This folder contains a build system and some extensions to integrate [Micro-XRCE-DSS](https://micro-xrce-dds.readthedocs.io/en/latest/), [Zephyr](https://www.zephyrproject.org/) and the [Olimex STM32 E407](https://www.olimex.com/Products/ARM/ST/STM32-E407/open-source-hardware) evaluation board.


## Required hardware

This sample app uses the following hardware:

| Item | |
|---------------|----------------------------------------------------------|
| Olimex STM32-E407 | [Link](https://www.olimex.com/Products/ARM/ST/STM32-E407/open-source-hardware) |
| Olimex ARM-USB-TINY-H | [Link](https://www.olimex.com/Products/ARM/JTAG/ARM-USB-TINY-H/) |
| USB-Serial Cable Female | [Link](https://www.olimex.com/Products/Components/Cables/USB-Serial-Cable/USB-Serial-Cable-F/) |


Olimex STM32-E407 features a STM32F407 Cortex-M4 microcontroller. It has 1MB Flash and 196 kB RAM of which 64 kB are core coupled (CCM SRAM). Other features of the chip are:

- Three 12 bits @ 2.4MHz ADC
- Two 12 bits DAC
- USB OTG FS
- USB OTG HS
- Ethernet
- 14 timers
- 3 SPI
- 3 I2C
- 2 CAN
- 114 GPIOs

Most of these peripherals are mapped into Olimex board headers. The BSP and peripheral driver of this sample app relies on Zephyr RTOS. The out-of-the-box configuration included in the port packages configures the minimal communication and debugging peripheral required for Micro XRCE-DDS.

## Building

Install some dependecies and the ARM compiler:

```bash
sudo apt update
apt install python3-pip dfu-util wget
pip3 install west

# Install last version of CMake
wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | sudo apt-key add -
echo "deb https://apt.kitware.com/ubuntu/ bionic main" > /etc/apt/sources.list.d/kitware.list
sudo apt update
sudo apt install cmake -y
```


Clone this repository:

```bash
git clone https://github.com/eProsima/Micro-XRCE-DDS-Apps
```

Update Micro XRCE-DDS submodule:

```bash
git submodule init
git submodule update
```

Create a Zephyr project inside the folder:

```bash
cd Zephyr
west init zephyrproject
cd zephyrproject
west update
cd ..
```

Install Zephyr requirements and toolchains:

```bash
# Install requeriments
pip3 install -r zephyrproject/zephyr/scripts/requirements.txt

# Install toolchain
export TOOLCHAIN_VERSION=zephyr-toolchain-arm-0.11.2-setup.run
wget https://github.com/zephyrproject-rtos/sdk-ng/releases/download/v0.11.2/$TOOLCHAIN_VERSION
chmod +x $TOOLCHAIN_VERSION
./$TOOLCHAIN_VERSION -- -d $(pwd)/zephyr-sdk -y
```

Set the environment variables and source the Zephyr project:

```bash
export ZEPHYR_TOOLCHAIN_VARIANT=zephyr
export ZEPHYR_SDK_INSTALL_DIR=$(pwd)/zephyr-sdk
source zephyrproject/zephyr/zephyr-env.sh
```

Build the firmware:

```bash
west build -b olimex_stm32_e407 -p auto . -- -G'Unix Makefiles' -DCMAKE_VERBOSE_MAKEFILE=ON
```

Flash the Olimex board using the JTAG adapter:

```bash
openocd -f interface/ftdi/olimex-arm-usb-tiny-h.cfg -f target/stm32f4x.cfg -c init -c "reset halt" -c "flash write_image erase build/microxrceddsapp.bin 0x08000000" -c "reset" -c "exit"
```