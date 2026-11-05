# Bare-Metal Coding: STM32 From Scratch

![C](https://img.shields.io/badge/Language-C-blue.svg)
![Build](https://img.shields.io/badge/Build-Make-orange.svg)
![MCU](https://img.shields.io/badge/MCU-STM32F108-green.svg)
![Status](https://img.shields.io/badge/Status-Active_Learning-success.svg)

## Documents

| Files                  | Description                                                                             |
| ---------------------- | --------------------------------------------------------------------------------------- |
| [README.md](README.md) | Main project overview, hardware information, firmware features, and operating principle |
| [Docs](./docs/)        | Contains documentation & reference manuals                                              |
| [Inc](./Inc/)          | Header files, register maps macros and peripheral drivers                               |
| [Src](./Src/)          | Source C files (main.c, startup.c)                                                      |

## Project Overview

This project is a deep dive into **bare-metal firmware engineering**, focusing on the STM32F108 microcontroller. By building everything from the ground up, this project serves as a foundational guide to understanding microcontrollers, bridging the gap between hardware architecture and low-level C programming. \
**The project demonstrates embedded firmware development concepts including:**

- STM32 Architecture
- Boot Process & Startup Code
- Linker Scripts
- Build Systems
- Register-Level Programming
- Learning how to make your own RTOS in STM32

## Features

**In this projects, we will dive into peripherals of STM32, including:**

- GPIO: Write, read, set pins
- TIMER: From PWM to making delay function using TIMER
- USART: Recieve and Sending strings
- I2C: Connect to OLED, recieve and sending packets
- ADC: Reading and converting raw data from sensors
- And many more......

**Beside that, we will learn how to use `Systick`, consist of:**

- How to make `delay` function
- Learn how to make a custom RTOS from scheduling to making `OS_Delay()`

## Prerequisites & Tools

To build and flash this project, you will need the following tools:

- **Compiler:** [GNU Arm Embedded Toolchain](https://developer.arm.com/downloads/-/gnu-rm) (`arm-none-eabi-gcc`)
- **Build System:** `Make`
- **Flashing/Debugging:** OpenOCD & ST-Link v2 (or your preferred debugger)

## Quick Start

1. Clone the repository:
   ```bash
   git clone https://github.com/KimThanhNguyen-hcmus/Bare_Metal_Coding_Stm32.git
   cd Bare_Metal_Coding_Stm32
   ```
2. Build the project:
   ```bash
   make all
   ```
   or
   ```bash
   mingw32-make
   ```
3. Flash the firmware to the board:
   ```bash
   make flash
   ```
   or
   ```bash
   mingw32-make flash
   ```

## Developer's Diary

I document my daily progress, bugs encountered, and architectural concepts learned on Notion. Feel free to follow along with my thought process:
[Bare-metal-coding](https://www.notion.so/Bare-metal-programming-155984656d2c836f813c01230064508a?source=copy_link)
I also document my daily progress, bugs and approachs about RTOS in Notion.
[RTOS from scratch](https://www.notion.so/RTOS-from-scratch-English-ver-33b984656d2c806c9579eb40ed18872a?source=copy_link)

<h3>Contact Me</h3>
<p>
  <a href="[https://github.com/NguyenKimThanh](https://github.com/KimThanhNguyen1409)">
    <img src="https://img.shields.io/badge/GitHub-KimThanhNguyen-181717?style=flat&logo=github&logoColor=white"/>
  </a>
  
  <a href="https://www.linkedin.com/in/nguyenkimthanh1409/">
    <img src="https://img.shields.io/badge/LinkedIn-Nguyễn%20Kim%20Thành-0A66C2?style=flat&logo=linkedin&logoColor=white"/>
  </a>
  
  <a href="mailto:nkimthanh47@gmail.com">
    <img src="https://img.shields.io/badge/Gmail-nkimthanh47%40gmail.com-EA4335?style=flat&logo=gmail&logoColor=white"/>
  </a>
</p>
