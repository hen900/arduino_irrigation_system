# arduino_irrigation_system
An indoor irrigation system that utilizes 4 moisture sensors, 4 pumps, and an arduino as a controller.

## Pictures and Diagrams ##
Pictures from my installed system are included in this repository along with a complete wiring diagram

## Arduino Code ##
The project is written for Arduino in C++. For more information, see the comments on the program.

## Moisture Sensor Calibration ##
In order to get a normalized reading from each sensor, they must be calibrated. The testing function can be used to take readings in air and while in water. The values outputted for water and air for each sensor should then be passed as arguments to the tray function
