#!/bin/bash


# eg --> "/dev/ttyUSB0"
MCP="/dev/ttyUSB0"


FN="Transmitter.ino"


# eg --> "esp32:esp32:esp32-poe-iso"
Board="esp32:esp32:esp32-poe-iso" 

arduino-cli compile --fqbn $Board $FN

arduino-cli upload -p $MCP --fqbn $Board $FN

