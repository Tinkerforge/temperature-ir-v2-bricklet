#!/usr/bin/env python
# -*- coding: utf-8 -*-

HOST = "localhost"
PORT = 4223
UID = "XYZ" # Change XYZ to the UID of your Temperature IR Bricklet 2.0

from tinkerforge.ip_connection import IPConnection
from tinkerforge.bricklet_temperature_ir_v2 import BrickletTemperatureIRV2

if __name__ == "__main__":
    ipcon = IPConnection() # Create IP connection
    tir = BrickletTemperatureIRV2(UID, ipcon) # Create device object

    ipcon.connect(HOST, PORT) # Connect to brickd
    # Don't use device before ipcon is connected

    # Get current ambient temperature
    ambient_temperature = tir.get_ambient_temperature()
    print("Ambient Temperature: " + str(ambient_temperature/10.0) + " °C")

    # Get current object temperature
    object_temperature = tir.get_object_temperature()
    print("Object Temperature: " + str(object_temperature/10.0) + " °C")

    raw_input("Press key to exit\n") # Use input() in Python 3
    ipcon.disconnect()
