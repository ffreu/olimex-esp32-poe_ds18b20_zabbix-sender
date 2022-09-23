# About
This is a small program written for an OLIMEX ESP32-POE-ISO board with a connected DS18B20 temperature sensor.
It reads the temperature from the sensor and sends it to a zabbix server with a configured trapper.

## How to run
 - Install Arduino IDE
 - Open the .ino file in the arduino IDE
 - Import esp32 boards using the board manager (ctrl + shift + b).
 - You can install the OneWire and DallasTemperature libraries using Arduino's library manager (ctrl + shift + i).
 - The ESP32ZabbixSender library needs to be added manually to your {Arduino IDE}\libraries\ folder
	 - (create a folder for it and put the .cpp and .h from the following git repository in it: [ESP32ZabbixSender](https://github.com/ffreu/ESP32ZabbixSender))
 - edit macros and variables to fit your needs
 - Select board and connected COM port and click on run