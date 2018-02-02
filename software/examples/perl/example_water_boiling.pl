#!/usr/bin/perl

use Tinkerforge::IPConnection;
use Tinkerforge::BrickletTemperatureIRV2;

use constant HOST => 'localhost';
use constant PORT => 4223;
use constant UID => 'XYZ'; # Change XYZ to the UID of your Temperature IR Bricklet 2.0

# Callback subroutine for object temperature reached callback
sub cb_object_temperature
{
    my ($temperature) = @_;

    print "Object Temperature: " . $temperature/10.0 . " °C\n";
    print "The water is boiling!\n";
}

my $ipcon = Tinkerforge::IPConnection->new(); # Create IP connection
my $tir = Tinkerforge::BrickletTemperatureIRV2->new(&UID, $ipcon); # Create device object

$ipcon->connect(&HOST, &PORT); # Connect to brickd
# Don't use device before ipcon is connected

# Set emissivity to 0.98 (emissivity of water, 65535 * 0.98 = 64224.299)
$tir->set_emissivity(64224);

# Register object temperature reached callback to subroutine cb_object_temperature
$tir->register_callback($tir->CALLBACK_OBJECT_TEMPERATURE, 'cb_object_temperature');

# Configure threshold for object temperature "greater than 100 °C"
# with a debounce period of 10s (10000ms)
$tir->set_object_temperature_callback_configuration(10000, 0, '>', 100*10, 0);

print "Press key to exit\n";
<STDIN>;
$ipcon->disconnect();
