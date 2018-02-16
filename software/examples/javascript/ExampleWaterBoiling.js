var Tinkerforge = require('tinkerforge');

var HOST = 'localhost';
var PORT = 4223;
var UID = 'XYZ'; // Change XYZ to the UID of your Temperature IR Bricklet 2.0

var ipcon = new Tinkerforge.IPConnection(); // Create IP connection
var tir = new Tinkerforge.BrickletTemperatureIRV2(UID, ipcon); // Create device object

ipcon.connect(HOST, PORT,
    function (error) {
        console.log('Error: ' + error);
    }
); // Connect to brickd
// Don't use device before ipcon is connected

ipcon.on(Tinkerforge.IPConnection.CALLBACK_CONNECTED,
    function (connectReason) {
        // Set emissivity to 0.98 (emissivity of water, 65535 * 0.98 = 64224.299)
        tir.setEmissivity(64224);

        // Configure threshold for object temperature "greater than 100 °C"
        // with a debounce period of 10s (10000ms)
        tir.setObjectTemperatureCallbackConfiguration(10000, false, '>', 100*10, 0);
    }
);

// Register object temperature reached callback
tir.on(Tinkerforge.BrickletTemperatureIRV2.CALLBACK_OBJECT_TEMPERATURE,
    // Callback function for object temperature reached callback
    function (temperature) {
        console.log('Object Temperature: ' + temperature/10.0 + ' °C');
        console.log('The water is boiling!');
    }
);

console.log('Press key to exit');
process.stdin.on('data',
    function (data) {
        ipcon.disconnect();
        process.exit(0);
    }
);
