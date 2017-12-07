#!/bin/sh
# Connects to localhost:4223 by default, use --host and --port to change this

uid=XYZ # Change XYZ to the UID of your Temperature IR Bricklet 2.0

# Set emissivity to 0.98 (emissivity of water, 65535 * 0.98 = 64224.299)
tinkerforge call temperature-ir-v2-bricklet $uid set-emissivity 64224

# Handle incoming object temperature reached callbacks (parameter has unit °C/10)
tinkerforge dispatch temperature-ir-v2-bricklet $uid object-temperature\
 --execute "echo Object Temperature: {temperature} °C/10. The water is boiling!" &

# Configure threshold for object temperature "greater than 100 °C" (unit is °C/10)
# with a debounce period of 10s (10000ms)
tinkerforge call temperature-ir-v2-bricklet $uid set-object-temperature-callback-configuration 10000 false threshold-option-greater 1000 0

echo "Press key to exit"; read dummy

kill -- -$$ # Stop callback dispatch in background
