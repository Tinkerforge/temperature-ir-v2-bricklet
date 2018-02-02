#!/bin/sh
# Connects to localhost:4223 by default, use --host and --port to change this

uid=XYZ # Change XYZ to the UID of your Temperature IR Bricklet 2.0

# Handle incoming object temperature callbacks
tinkerforge dispatch temperature-ir-v2-bricklet $uid object-temperature &

# Set period for object temperature callback to 1s (1000ms) without a threshold
tinkerforge call temperature-ir-v2-bricklet $uid set-object-temperature-callback-configuration 1000 false threshold-option-off 0 0

echo "Press key to exit"; read dummy

kill -- -$$ # Stop callback dispatch in background
