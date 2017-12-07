function matlab_example_simple()
    import com.tinkerforge.IPConnection;
    import com.tinkerforge.BrickletTemperatureIRV2;

    HOST = 'localhost';
    PORT = 4223;
    UID = 'XYZ'; % Change XYZ to the UID of your Temperature IR Bricklet 2.0

    ipcon = IPConnection(); % Create IP connection
    tir = handle(BrickletTemperatureIRV2(UID, ipcon), 'CallbackProperties'); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Get current ambient temperature (unit is °C/10)
    ambientTemperature = tir.getAmbientTemperature();
    fprintf('Ambient Temperature: %g °C\n', ambientTemperature/10.0);

    % Get current object temperature (unit is °C/10)
    objectTemperature = tir.getObjectTemperature();
    fprintf('Object Temperature: %g °C\n', objectTemperature/10.0);

    input('Press key to exit\n', 's');
    ipcon.disconnect();
end
