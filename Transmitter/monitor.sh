#!/bin/bash

# Port to be used (eg: "/dev/ttyUSB0")
MCP="/dev/ttyUSB1" 

putty $MCP -serial -sercfg 115200
