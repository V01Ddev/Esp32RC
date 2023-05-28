#!/bin/bash

# Port to be used (eg: "/dev/ttyUSB0")
MCP="/dev/ttyUSB0" 

putty $MCP -serial -sercfg 115200
