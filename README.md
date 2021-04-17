# DS1307 C Driver
## Introduction
Here is the MCU independent, lightweight and portable C driver for DS1307 Real Time Clock. "The DS1307 serial real-time clock (RTC) is a low-power, full binary-coded decimal (BCD) clock/calendar plus 56 bytes of NV SRAM". The driver is written using standard C straight from Maxim Integrated datasheet and is not influenced by other libraries. CPU intensive tasks are avoided, and mathematical expressions are written using bitwise operators. In order to port this library to any MCU, you have to tweak the low level API file.

## Features
The DS1307 has three main features:
•	Real time clock with stop/start and AM/PM capability.
•	Square wave generator with 4 programmable frequencies: 1 Hz, 4.096 KHz, 8.192 KHz and 32.768 KHz.
•	56 Bytes general purpose RAM.
All of these features are utilized in this library. You can initialize DS1307 using DS1307_init() (with automatic reset, since DS1307 is battery backed and if MCU is reset or out of power, DS1307 continues to keep time so there’s no need to reset the DS1307 everytime our firmware starts). You can start or stop the square wave generator using DS1307_square_wave(). You can also save a snapshot of time inside DS1307 general purpose RAM, for example in the case of an event happening, using DS1307_snapshot_save().

## How to use

