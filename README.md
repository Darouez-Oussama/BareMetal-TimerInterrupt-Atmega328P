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

### Timer Interrupt Configuration

The core of this project is the `timerInterruptSetup` function, which configures Timer1 on the ATmega328P to generate periodic interrupts for sine wave sampling. Here's a detailed, step-by-step explanation of how it works:

- **Purpose**: The function sets up Timer1 to trigger an interrupt at a specified frequency (e.g., 2Hz), allowing the microcontroller to generate and transmit sine wave data at regular intervals.

- **Input Parameter**:

  - `frequency` (float): The desired interrupt frequency in Hertz. The function calculates the appropriate timer settings to achieve this frequency.

- **Key Constants**:

  - `clockFrequency = 16000000` (16 MHz): The ATmega328P's clock frequency.
  - `prescalerValues[5] = {1, 8, 64, 256, 1024}`: Available prescaler values to divide the clock frequency, reducing the timer's effective frequency.

- **Algorithm**:

  1. **Calculate Timer Settings**:

     - The function iterates through the prescaler values to find a combination that results in a `compareValue` (the value to compare against the timer counter) less than 65536 (the maximum value for a 16-bit timer).
     - The formula used is: `compareValue = (clockFrequency / (frequency * prescaler)) - 1`.
     - This ensures the timer overflows at the desired interval, triggering an interrupt.

  2. **Disable Interrupts**:

     - `cli()`: Disables all interrupts to prevent race conditions during timer setup.
     - `TCCR1A = 0` and `TCCR1B = 0`: Resets Timer1 control registers.
     - `TCNT1 = 0`: Resets the timer counter to zero.

  3. **Set Compare Value**:

     - `OCR1A = (uint16_t)compareValue`: Loads the calculated compare value into the Output Compare Register (OCR1A), which determines when the interrupt triggers.

  4. **Configure Timer Mode**:

     - `TCCR1B |= (1 << WGM12)`: Sets Timer1 to Clear Timer on Compare Match (CTC) mode, where the timer resets to zero when it matches `OCR1A`.

  5. **Set Prescaler**:

     - Uses a `switch` statement to set the prescaler bits in `TCCR1B` based on the selected `prescaler` value:
       - `CS10` for 1x prescaler.
       - `CS11` for 8x prescaler.
       - `CS10 | CS11` for 64x prescaler.
       - `CS12` for 256x prescaler.
       - `CS10 | CS12` for 1024x prescaler.
     - This divides the clock frequency, controlling the timer's speed.

  6. **Enable Interrupt**:

     - `TIMSK1 |= (1 << OCIE1A)`: Enables the Output Compare Match A interrupt for Timer1.

  7. **Re-enable Interrupts**:

     - `sei()`: Re-enables all interrupts, allowing the timer interrupt to fire when the counter matches `OCR1A`.

- **Outcome**: The timer interrupt triggers at the specified `frequency`, and the Interrupt Service Routine (ISR) (defined elsewhere, e.g., in `main.cpp`) handles the sine wave generation and serial transmission.

## Code Structure

- `src/main.cpp`: Main program file containing setup, loop, and ISR implementation
- `src/timerInterrupt.h`: Timer configuration and setup functions

## How It Works

1. The timer interrupt is configured to trigger at 2Hz frequency.
2. On each interrupt:
   - A sine wave point is calculated using the formula: `sin(i * 50 / 360.0)`.
   - The value is sent over serial communication.
   - Termination characters (CR+LF) are appended.
   - The counter is incremented for the next sample.

## Usage

1. Upload the code to your Arduino board.
2. Open MATLAB and configure it to read from the appropriate serial port.
3. The sine wave data will be transmitted continuously for plotting.

## Serial Communication Format

The sine wave data is transmitted over the serial port at a baud rate of 9600. Each data point follows this format:

```
[sine_value]\r\n
```

### Detailed Explanation

- **\[sine_value\]**: The calculated sine wave value, a floating-point number between -1 and 1, representing the amplitude at the current sample point. This value is derived from the formula `sin(i * 50 / 360.0)`, where `i` is a counter incremented on each interrupt, and the division by 360 scales the angle to degrees.
- `\r`: Carriage return (ASCII 13), which moves the cursor to the beginning of the line. This ensures the next data point overwrites the previous line in some terminal displays.
- `\n`: Line feed (ASCII 10), which moves the cursor to the next line. Together with `\r`, this pair (CR+LF) acts as a terminator, signaling the end of a data point and preparing for the next.

### MATLAB Integration

To visualize the data in MATLAB:

1. Use the `serialport` function to open the Arduino's serial port (e.g., `s = serialport("COM3", 9600);`).
2. Read data continuously using `readline(s)` to capture each `[sine_value]\r\n` line.
3. Plot the values in real-time using a loop and `plot` function, updating the graph as new data arrives.

### Example Data

A sample serial output might look like:

```
0.0000\r\n
0.0698\r\n
0.1392\r\n
...
```

This sequence represents the sine wave's progression over time, with each value sent at the interrupt frequency (e.g., every 0.5 seconds for 2Hz).
