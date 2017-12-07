#!/usr/bin/env ruby
# -*- ruby encoding: utf-8 -*-

require 'tinkerforge/ip_connection'
require 'tinkerforge/bricklet_temperature_ir_v2'

include Tinkerforge

HOST = 'localhost'
PORT = 4223
UID = 'XYZ' # Change XYZ to the UID of your Temperature IR Bricklet 2.0

ipcon = IPConnection.new # Create IP connection
tir = BrickletTemperatureIRV2.new UID, ipcon # Create device object

ipcon.connect HOST, PORT # Connect to brickd
# Don't use device before ipcon is connected

# Register object temperature callback (parameter has unit °C/10)
tir.register_callback(BrickletTemperatureIRV2::CALLBACK_OBJECT_TEMPERATURE) do |temperature|
  puts "Object Temperature: #{temperature/10.0} °C"
end

# Set period for object temperature callback to 1s (1000ms) without a threshold
tir.set_object_temperature_callback_configuration 1000, false, 'x', 0, 0

puts 'Press key to exit'
$stdin.gets
ipcon.disconnect
