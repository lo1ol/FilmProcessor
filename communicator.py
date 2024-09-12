#!/usr/bin/env python

import serial
import glob
import os
import json
import sys

CHUNK_SIZE=60


if len(sys.argv) < 2:
    print("Command has to be specified: 'load' or 'dump'", file=sys.stderr)
    exit(1)

def getPort():
    return glob.glob('/dev/cu.usb*')[0]

def getPrograms():
    script_dir = os.path.dirname(os.path.realpath(__file__))
    with open(os.path.join(script_dir, "programs.json")) as f:
        return json.loads(f.read())

arduino = serial.Serial(port=getPort(), baudrate=9600, timeout=1)

def upload():
    for prog in getPrograms():
        minifiedProg = json.dumps(prog, separators=(',', ":"))
        chunks = [minifiedProg[i:i + CHUNK_SIZE] for i in range(0, len(minifiedProg), CHUNK_SIZE)]

        chunkId = 0
        while True:
            data = arduino.readline().decode("utf-8").strip()
            print(data, file=sys.stderr)
            if data == "Ready to read chunk":
                if chunkId == len(chunks):
                    arduino.write(bytes("\r\n", 'utf-8'))
                    break
                arduino.write(bytes(chunks[chunkId], 'utf-8'))
                print("Chunk: '" + chunks[chunkId] + "' was sent", file=sys.stderr)
                chunkId += 1

    while True:
        data = arduino.readline().decode("utf-8").strip()
        print(data, file=sys.stderr)
        if (len(data) == 0):
            break

def dump():
    res = ""

    while True:
        data = arduino.readline().decode("utf-8")
        if len(data) == 0:
            print("Got bad input:\n" + res, file=sys.stderr)
            exit(1)

        print("Chunk received", file=sys.stderr);
        res += data;

        # try to read until we get readable json
        try:
            print(json.dumps(json.loads(res), sort_keys=True, indent=4))
            return
        except:
            pass

msg = arduino.readline().decode("utf-8").strip()
if msg != "Started":
    print("Got unexpected string: " + msg, file=sys.stderr)
    exit(1)

if sys.argv[1] == "load":
    arduino.write(bytes("Loading", 'utf-8'))
    print("Get previous programs", file=sys.stderr)
    dump()
    print("\nLoad program", file=sys.stderr)
    upload();

elif sys.argv[1] == "dump":
    arduino.write(bytes("Exporting", 'utf-8'))
    dump();
