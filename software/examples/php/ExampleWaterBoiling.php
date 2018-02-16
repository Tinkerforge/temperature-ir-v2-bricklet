<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletTemperatureIRV2.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletTemperatureIRV2;

const HOST = 'localhost';
const PORT = 4223;
const UID = 'XYZ'; // Change XYZ to the UID of your Temperature IR Bricklet 2.0

// Callback function for object temperature reached callback
function cb_objectTemperature($temperature)
{
    echo "Object Temperature: " . $temperature/10.0 . " °C\n";
    echo "The water is boiling!\n";
}

$ipcon = new IPConnection(); // Create IP connection
$tir = new BrickletTemperatureIRV2(UID, $ipcon); // Create device object

$ipcon->connect(HOST, PORT); // Connect to brickd
// Don't use device before ipcon is connected

// Set emissivity to 0.98 (emissivity of water, 65535 * 0.98 = 64224.299)
$tir->setEmissivity(64224);

// Register object temperature reached callback to function cb_objectTemperature
$tir->registerCallback(BrickletTemperatureIRV2::CALLBACK_OBJECT_TEMPERATURE,
                       'cb_objectTemperature');

// Configure threshold for object temperature "greater than 100 °C"
// with a debounce period of 10s (10000ms)
$tir->setObjectTemperatureCallbackConfiguration(10000, FALSE, '>', 100*10, 0);

echo "Press ctrl+c to exit\n";
$ipcon->dispatchCallbacks(-1); // Dispatch callbacks forever

?>
