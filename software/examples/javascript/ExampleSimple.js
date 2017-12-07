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
        // Get current ambient temperature (unit is °C/10)
        tir.getAmbientTemperature(
            function (ambientTemperature) {
                console.log('Ambient Temperature: ' + ambientTemperature/10.0 + ' °C');
            },
            function (error) {
                console.log('Error: ' + error);
            }
        );

        // Get current object temperature (unit is °C/10)
        tir.getObjectTemperature(
            function (objectTemperature) {
                console.log('Object Temperature: ' + objectTemperature/10.0 + ' °C');
            },
            function (error) {
                console.log('Error: ' + error);
            }
        );
    }
);

console.log('Press key to exit');
process.stdin.on('data',
    function (data) {
        ipcon.disconnect();
        process.exit(0);
    }
);
