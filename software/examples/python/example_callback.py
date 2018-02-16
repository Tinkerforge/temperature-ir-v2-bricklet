#!/usr/bin/env python
# -*- coding: utf-8 -*-

HOST = "localhost"
PORT = 4223
UID = "XYZ" # Change XYZ to the UID of your Temperature IR Bricklet 2.0

from tinkerforge.ip_connection import IPConnection
from tinkerforge.bricklet_temperature_ir_v2 import BrickletTemperatureIRV2

# Callback function for object temperature callback
def cb_object_temperature(temperature):
    print("Object Temperature: " + str(temperature/10.0) + " °C")

if __name__ == "__main__":
    ipcon = IPConnection() # Create IP connection
    tir = BrickletTemperatureIRV2(UID, ipcon) # Create device object

    ipcon.connect(HOST, PORT) # Connect to brickd
    # Don't use device before ipcon is connected

    # Register object temperature callback to function cb_object_temperature
    tir.register_callback(tir.CALLBACK_OBJECT_TEMPERATURE, cb_object_temperature)

    # Set period for object temperature callback to 1s (1000ms) without a threshold
    tir.set_object_temperature_callback_configuration(1000, False, "x", 0, 0)

    raw_input("Press key to exit\n") # Use input() in Python 3
    ipcon.disconnect()
