# Micro-XRCE-DDS + FreeRTOS + Olimex STM32 E407 Sample app

This folder contains a build system and some extensions to integrate [Micro-XRCE-DDS](https://micro-xrce-dds.readthedocs.io/en/latest/), [FreeRTOS](https://www.freertos.org/) and the [Olimex STM32 E407](https://www.olimex.com/Products/ARM/ST/STM32-E407/open-source-hardware) evaluation board.


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

Most of these peripherals are mapped into Olimex board headers. The FreeRTOS BSP of this evaluation board relies on a [STM32CubeMX](https://www.st.com/en/development-tools/stm32cubemx.html) generated project, so HAL and low-level configuration can be tuned to specific requirements. The out-of-the-box HAL configuration included in the port packages configures the minimal communication and debugging peripheral required for Micro XRCE-DDS.


## Prepare the environment

Install some dependecies and the ARM compiler:

```
sudo apt update
sudo apt install build-essential cmake git openocd gcc-arm-none-eabi usbutils
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

Install STM32CubeMX from ST webpage: [download here](https://www.st.com/en/development-tools/stm32cubemx.html).

Inside STM32CubeMX load `FreeRTOS/sample_project.ioc` and generate code.

Create some required POSIX files:

```bash
cd FreeRTOS
touch Middlewares/Third_Party/FreeRTOS_POSIX/include/FreeRTOS_POSIX/sys/time.h
touch Middlewares/Third_Party/FreeRTOS_POSIX/include/FreeRTOS_POSIX/sys/timeval.h # This file can be empty
```

The content of `FreeRTOS/Middlewares/Third_Party/FreeRTOS_POSIX/include/FreeRTOS_POSIX/sys/time.h` should be:

```
#ifndef _FREERTOS_POSIX_SYSTIME_H_
#define _FREERTOS_POSIX_SYSTIME_H_

struct timeval {
  long    tv_sec;         /* seconds */
  long    tv_usec;        /* and microseconds */
};

#endif /* ifndef _FREERTOS_POSIX_SYSTIME_H_ */
```

In `FreeRTOS/Inc/FreeRTOSConfig.h` add these two lines in the `USER CODE` section (line 45):

```c
/* USER CODE BEGIN Includes */
/* Section where include file can be added */
#define configUSE_POSIX_ERRNO 1
#define INCLUDE_xTaskGetHandle 1
/* USER CODE END Includes */
```


Overwrite `main.c`. This file add the Micro XRCE-DDS task initialization, you can analyze it at line 373:

```bash
cp -rf main_template.c Src/main.c
```

## Building

Build Micro-XRCE-DDS-Client library:

```bash
make libmicroxrcedds
```

Set the Micro XRCE-DDS Agent IP address and build the firmware:

```bash
make UXRCEDDS_AGENT_IP=[Agent IP] UXRCEDDS_AGENT_PORT=[Agent Port]
```

Flash the Olimex board using the JTAG adapter:

```bash
openocd -f interface/ftdi/olimex-arm-usb-tiny-h.cfg -f target/stm32f4x.cfg -c init -c "reset halt" -c "flash write_image erase build/sample_project.bin 0x08000000" -c "reset" -c "exit"
```