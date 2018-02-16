<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletTemperatureIRV2.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletTemperatureIRV2;

const HOST = 'localhost';
const PORT = 4223;
const UID = 'XYZ'; // Change XYZ to the UID of your Temperature IR Bricklet 2.0

$ipcon = new IPConnection(); // Create IP connection
$tir = new BrickletTemperatureIRV2(UID, $ipcon); // Create device object

$ipcon->connect(HOST, PORT); // Connect to brickd
// Don't use device before ipcon is connected

// Get current ambient temperature
$ambient_temperature = $tir->getAmbientTemperature();
echo "Ambient Temperature: " . $ambient_temperature/10.0 . " °C\n";

// Get current object temperature
$object_temperature = $tir->getObjectTemperature();
echo "Object Temperature: " . $object_temperature/10.0 . " °C\n";

echo "Press key to exit\n";
fgetc(fopen('php://stdin', 'r'));
$ipcon->disconnect();

?>
