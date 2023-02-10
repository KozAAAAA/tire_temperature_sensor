# Tire Temperature Sensor v2.0

**TTS** (Tire Temperature Sensor) is a PCB designed to measure temperature quickly, at a distance, and without the need to touch the wheels. The 4x16 pixel IR camera is used to obtain the data. It communicates with the STM microprocessor via the I2C protocol. Data is then averaged out to a 1x8 array of unsigned ints and sent with the CAN Transceiver to the car’s CAN network. This array can be then read off of the telemetry system implemented in the PM-08 racing car.

<p align="center">
  <img src="/readme/3.jpg" alt="Sublime's custom image"/>
</p>

<p align="center">
  <img src="/readme/4.png" alt="Sublime's custom image"/>
</p>


## Applications

While driving, a tire heats up. Heat dissipates differently across its surface and its rate changes depending on many factors (E.g. weather conditions or reoccurring acceleration). As a tire has its maximal temperature, exceeding this value may weaken its structural strength and cause its destruction in the worst-case scenario. This IR Camera PCB makes it possible to monitor a tire across its whole surface and prevent any damage associated with the temperature. As TTS monitors every wheel out of 4, our crew can easily detect if tires wear unevenly or excessively. Tire temperature patterns inform whether the car is properly calibrated, as the out-of-tune suspension causes tires to heat up significantly faster. This may result in grip loss in these areas.

## Core features

* 4x16 pixel MLX90621 thermal camera
* STM32L4P5RET
* 24V to 3.3V buck converter
* 3.3V to 2.6V LDO
* 3.3V CAN transceiver
* Overcurrent and reverse polarity protection
* Status LEDs

## PCB

<p align="center">
  <img src="/readme/1.png" alt="Sublime's custom image"/>
</p>
<p align="center">
  <img src="/readme/2.png" alt="Sublime's custom image"/>
</p>
