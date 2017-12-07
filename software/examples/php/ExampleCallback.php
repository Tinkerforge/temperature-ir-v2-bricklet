<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletTemperatureIRV2.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletTemperatureIRV2;

const HOST = 'localhost';
const PORT = 4223;
const UID = 'XYZ'; // Change XYZ to the UID of your Temperature IR Bricklet 2.0

// Callback function for object temperature callback (parameter has unit °C/10)
function cb_objectTemperature($temperature)
{
    echo "Object Temperature: " . $temperature/10.0 . " °C\n";
}

$ipcon = new IPConnection(); // Create IP connection
$tir = new BrickletTemperatureIRV2(UID, $ipcon); // Create device object

$ipcon->connect(HOST, PORT); // Connect to brickd
// Don't use device before ipcon is connected

// Register object temperature callback to function cb_objectTemperature
$tir->registerCallback(BrickletTemperatureIRV2::CALLBACK_OBJECT_TEMPERATURE,
                       'cb_objectTemperature');

// Set period for object temperature callback to 1s (1000ms) without a threshold
$tir->setObjectTemperatureCallbackConfiguration(1000, FALSE, 'x', 0, 0);

echo "Press ctrl+c to exit\n";
$ipcon->dispatchCallbacks(-1); // Dispatch callbacks forever

?>
