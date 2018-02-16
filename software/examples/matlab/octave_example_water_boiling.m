function octave_example_water_boiling()
    more off;

    HOST = "localhost";
    PORT = 4223;
    UID = "XYZ"; % Change XYZ to the UID of your Temperature IR Bricklet 2.0

    ipcon = javaObject("com.tinkerforge.IPConnection"); % Create IP connection
    tir = javaObject("com.tinkerforge.BrickletTemperatureIRV2", UID, ipcon); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Set emissivity to 0.98 (emissivity of water, 65535 * 0.98 = 64224.299)
    tir.setEmissivity(64224);

    % Register object temperature reached callback to function cb_object_temperature
    tir.addObjectTemperatureCallback(@cb_object_temperature);

    % Configure threshold for object temperature "greater than 100 °C"
    % with a debounce period of 10s (10000ms)
    tir.setObjectTemperatureCallbackConfiguration(10000, false, ">", 100*10, 0);

    input("Press key to exit\n", "s");
    ipcon.disconnect();
end

% Callback function for object temperature reached callback
function cb_object_temperature(e)
    fprintf("Object Temperature: %g °C\n", e.temperature/10.0);
    fprintf("The water is boiling!\n");
end
