function octave_example_simple()
    more off;

    HOST = "localhost";
    PORT = 4223;
    UID = "XYZ"; % Change XYZ to the UID of your Temperature IR Bricklet 2.0

    ipcon = javaObject("com.tinkerforge.IPConnection"); % Create IP connection
    tir = javaObject("com.tinkerforge.BrickletTemperatureIRV2", UID, ipcon); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Get current ambient temperature
    ambientTemperature = tir.getAmbientTemperature();
    fprintf("Ambient Temperature: %g °C\n", ambientTemperature/10.0);

    % Get current object temperature
    objectTemperature = tir.getObjectTemperature();
    fprintf("Object Temperature: %g °C\n", objectTemperature/10.0);

    input("Press key to exit\n", "s");
    ipcon.disconnect();
end
