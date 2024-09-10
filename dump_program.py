#!/usr/bin/env python

import serial 
import glob
import json

def getPort():
    return glob.glob('/dev/cu.usb*')[0]

arduino = serial.Serial(port=getPort(), baudrate=9600)

res = ""

while True:
    data = arduino.readline().decode("utf-8").strip()
    if len(data) == 0:
        break;
    print("Chunk received");
    res += data;
    arduino.timeout = 2

print(json.dumps(json.loads(res), sort_keys=True, indent=4))
