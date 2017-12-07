#!/usr/bin/perl

use Tinkerforge::IPConnection;
use Tinkerforge::BrickletTemperatureIRV2;

use constant HOST => 'localhost';
use constant PORT => 4223;
use constant UID => 'XYZ'; # Change XYZ to the UID of your Temperature IR Bricklet 2.0

my $ipcon = Tinkerforge::IPConnection->new(); # Create IP connection
my $tir = Tinkerforge::BrickletTemperatureIRV2->new(&UID, $ipcon); # Create device object

$ipcon->connect(&HOST, &PORT); # Connect to brickd
# Don't use device before ipcon is connected

# Get current ambient temperature (unit is °C/10)
my $ambient_temperature = $tir->get_ambient_temperature();
print "Ambient Temperature: " . $ambient_temperature/10.0 . " °C\n";

# Get current object temperature (unit is °C/10)
my $object_temperature = $tir->get_object_temperature();
print "Object Temperature: " . $object_temperature/10.0 . " °C\n";

print "Press key to exit\n";
<STDIN>;
$ipcon->disconnect();
