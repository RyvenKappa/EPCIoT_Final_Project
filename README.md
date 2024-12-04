# Final Project repository for the Embedded Platforms and Communications for IoT

## _Introduction_

This project aims to provide support for all the biological data and gps location of a plant, in order to give producers and transport a way to analyze the correct treatment of the plant and to boost the capabilities of growth.

## _Hardware Elements_

All the necessary elements to develop the project was lent by the _Electronics and Telematics Deparment_ of the UPM:
- **Microcontroller**: a DISCO-L072CZ-LRWAN1 was used, this board includes a ARM Cortex M0+ STM32l072CZ core and all the I/O requirements for the development, as well as an ST-Link. It also provides a USER and a RESET button.
- **RGB Led** as output.
- **HW5P-1_2015__1** PhotoTransistor for brightness control.
- **Analog Soil Moisture** sensor from adafruit.
- **MMA8451Q-1** Accelerometer from adafruit to analyze falls and orientation change if needed.
- **TCS34725** Color Sensor to analyze the color of the leaves of the plant.
- **SI7021-A20** Temperature and Humidity sensor to analyze the air characteristics.
- **Adafruit FGPMMOPA6H** integration to provide GPS location and date/hour estimation.
## _Hardware Block Diagram_
![General drawio](https://github.com/user-attachments/assets/51e26a4e-9df1-4042-bc1a-eca1ddbf7d18)
## _Software Design_
![softdiagram](https://github.com/user-attachments/assets/eb67623b-5794-426e-a8fc-bf4fb2a8dc70)

## _Final Prototype Design_
![image](https://github.com/user-attachments/assets/7a0f49d2-3d71-446f-bc38-0bca12299a01)

Developep in Fall 2024.
