# Atmega328P Timer Interrupt Sine Wave Generator

This project implements a baremetal timer interrupt on an ATmega328P microcontroller (Arduino) to generate and transmit sine wave data over serial communication. The data can be visualized in real-time using MATLAB.

## Hardware Requirements

- Arduino board (with ATmega328P microcontroller)
- USB cable for serial communication
- Computer with MATLAB installed

## Features

- Precise timer interrupt implementation using Timer1
- Generates sine wave samples at a fixed frequency
- Transmits data over serial communication (9600 baud rate)
- Compatible with MATLAB for real-time plotting

## Implementation Details

The project uses the following key components:

- Timer1 configured for precise interrupt timing
- Serial communication at 9600 baud rate
- Sine wave generation with 360-degree resolution
- Carriage return (13) and line feed (10) characters as data terminators

## Code Structure

- `src/main.cpp`: Main program file containing setup, loop, and ISR implementation
- `src/timerInterrupt.h`: Timer configuration and setup functions

## How It Works

1. The timer interrupt is configured to trigger at 2Hz frequency
2. On each interrupt:
   - A sine wave point is calculated using the formula: sin(i*50/360.0)
   - The value is sent over serial communication
   - Termination characters (CR+LF) are appended
   - The counter is incremented for the next sample

## Usage

1. Upload the code to your Arduino board
2. Open MATLAB and configure it to read from the appropriate serial port
3. The sine wave data will be transmitted continuously for plotting

## Serial Communication Format

Each data point is sent in the following format:
```
[sine_value]\r\n
```