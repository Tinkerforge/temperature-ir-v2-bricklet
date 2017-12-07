#!/usr/bin/perl

use Tinkerforge::IPConnection;
use Tinkerforge::BrickletTemperatureIRV2;

use constant HOST => 'localhost';
use constant PORT => 4223;
use constant UID => 'XYZ'; # Change XYZ to the UID of your Temperature IR Bricklet 2.0

# Callback subroutine for object temperature callback (parameter has unit °C/10)
sub cb_object_temperature
{
    my ($temperature) = @_;

    print "Object Temperature: " . $temperature/10.0 . " °C\n";
}

my $ipcon = Tinkerforge::IPConnection->new(); # Create IP connection
my $tir = Tinkerforge::BrickletTemperatureIRV2->new(&UID, $ipcon); # Create device object

$ipcon->connect(&HOST, &PORT); # Connect to brickd
# Don't use device before ipcon is connected

# Register object temperature callback to subroutine cb_object_temperature
$tir->register_callback($tir->CALLBACK_OBJECT_TEMPERATURE, 'cb_object_temperature');

# Set period for object temperature callback to 1s (1000ms) without a threshold
$tir->set_object_temperature_callback_configuration(1000, 0, 'x', 0, 0);

print "Press key to exit\n";
<STDIN>;
$ipcon->disconnect();
