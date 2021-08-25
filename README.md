# ecg-analyzer
The project consists of the development of a device capable of non-invasively detecting blood flow in order to graph the measured signal in real time and calculate the heart rate.

Processing is done with a Cortex-M3 LPC1769 microcontroller, leveraging FreeRTOS operative system.

## Signal acquisition

The pulse sensor works by measuring the variation of light reflected off the skin tissue due to the pumped blood.

![pulse sensor](./images/pulse-sensor.png)

![pulse sensor signal](./images/pulse-sensor-signal.png)

## Signal drawing

For this we use a 320x240 pixels TFT display. Each column on the screen represents a sample. In order to shape the signal, the microcontroller plots a vertical line from the pixel corresponding to the value of the previous sample to the pixel that represents the value of the current sample. That is, the waveform is achieved by a succession of vertical bars.

![signal representation](./images/signal-representation.png)

![signal drawing](./images/signal-drawing.jpg)
