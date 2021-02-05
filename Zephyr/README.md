# Micro XRCE-DDS + Zephyr + Olimex STM32 E407 Sample app

This folder contains a build system and some extensions to integrate [Micro XRCE-DSS](https://micro-xrce-dds.readthedocs.io/en/latest/), [Zephyr](https://www.zephyrproject.org/) and the [Olimex STM32 E407](https://www.olimex.com/Products/ARM/ST/STM32-E407/open-source-hardware) evaluation board.


## Required hardware

This sample app uses the following hardware:

| Item | |
|---------------|----------------------------------------------------------|
| Olimex STM32-E407 | [Link](https://www.olimex.com/Products/ARM/ST/STM32-E407/open-source-hardware) |
| Olimex ARM-USB-TINY-H | [Link](https://www.olimex.com/Products/ARM/JTAG/ARM-USB-TINY-H/) |
| USB-Serial Cable Female | [Link](https://www.olimex.com/Products/Components/Cables/USB-Serial-Cable/USB-Serial-Cable-F/) |

## Building

Install dependencies and the ARM compiler:

```bash
sudo apt update
sudo apt install -y python3-pip dfu-util wget lsb-release git ninja-build
pip3 install west

# Install last version of CMake
wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | sudo apt-key add -
echo "deb https://apt.kitware.com/ubuntu/ $(lsb_release -cs) main" > /etc/apt/sources.list.d/kitware.list
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
export TOOLCHAIN_VERSION=0.11.4
export TOOLCHAIN_FILE_NAME=zephyr-toolchain-arm-$TOOLCHAIN_VERSION-setup.run
wget https://github.com/zephyrproject-rtos/sdk-ng/releases/download/v$TOOLCHAIN_VERSION/$TOOLCHAIN_FILE_NAME
chmod +x $TOOLCHAIN_FILE_NAME
./$TOOLCHAIN_FILE_NAME -- -rc -y -d $(pwd)/zephyr-sdk
rm -rf $TOOLCHAIN_FILE_NAME
```

Set the environment variables and source the Zephyr project:

```bash
export ZEPHYR_TOOLCHAIN_VARIANT=zephyr
export ZEPHYR_SDK_INSTALL_DIR=$(pwd)/zephyr-sdk
source zephyrproject/zephyr/zephyr-env.sh
```

Build the firmware:

```bash
west build -b olimex_stm32_e407 -p auto . 
```

Flash the Olimex board using the JTAG adapter:

```bash
openocd -f interface/ftdi/olimex-arm-usb-tiny-h.cfg -f target/stm32f4x.cfg -c init -c "reset halt" -c "flash write_image erase build/microxrceddsapp.bin 0x08000000" -c "reset" -c "exit"
```
