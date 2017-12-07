function octave_example_callback()
    more off;

    HOST = "localhost";
    PORT = 4223;
    UID = "XYZ"; % Change XYZ to the UID of your Temperature IR Bricklet 2.0

    ipcon = javaObject("com.tinkerforge.IPConnection"); % Create IP connection
    tir = javaObject("com.tinkerforge.BrickletTemperatureIRV2", UID, ipcon); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Register object temperature callback to function cb_object_temperature
    tir.addObjectTemperatureCallback(@cb_object_temperature);

    % Set period for object temperature callback to 1s (1000ms) without a threshold
    tir.setObjectTemperatureCallbackConfiguration(1000, false, "x", 0, 0);

    input("Press key to exit\n", "s");
    ipcon.disconnect();
end

% Callback function for object temperature callback (parameter has unit °C/10)
function cb_object_temperature(e)
    fprintf("Object Temperature: %g °C\n", e.temperature/10.0);
end
