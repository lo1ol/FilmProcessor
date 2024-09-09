#!/usr/bin/env python

import serial 
import glob
import os
import json

CHUNK_SIZE=60

def getPort():
    return glob.glob('/dev/cu.usb*')[0]

def getPrograms():
    script_dir = os.path.dirname(os.path.realpath(__file__))
    with open(os.path.join(script_dir, "programs.json")) as f:
        return json.loads(f.read())


arduino = serial.Serial(port=getPort(), baudrate=9600)

for prog in getPrograms():
    minifiedProg = json.dumps(prog, separators=(',', ":"))
    chunks = [minifiedProg[i:i + CHUNK_SIZE] for i in range(0, len(minifiedProg), CHUNK_SIZE)]

    chunkId = 0
    while True:
        data = arduino.readline().decode("utf-8").strip()
        print(data)
        if data == "Ready to read chunk":
            if chunkId == len(chunks):
                arduino.write(bytes("\r\n", 'utf-8'))
                break
            arduino.write(bytes(chunks[chunkId], 'utf-8'))
            print("Chunk: '" + chunks[chunkId] + "' was sent")
            chunkId += 1

arduino.timeout = 2

while True:
    data = arduino.readline().decode("utf-8").strip()
    print(data)
    if (len(data) == 0):
        break
