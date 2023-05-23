#!/bin/bash
MCP="/dev/ttyUSB0" # Port to be used (eg: "/dev/ttyUSB0")


putty $MCP -serial -sercfg 115200
