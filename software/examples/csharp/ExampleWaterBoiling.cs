using System;
using Tinkerforge;

class Example
{
	private static string HOST = "localhost";
	private static int PORT = 4223;
	private static string UID = "XYZ"; // Change XYZ to the UID of your Temperature IR Bricklet 2.0

	// Callback function for object temperature reached callback (parameter has unit °C/10)
	static void ObjectTemperatureCB(BrickletTemperatureIRV2 sender, short temperature)
	{
		Console.WriteLine("Object Temperature: " + temperature/10.0 + " °C");
		Console.WriteLine("The water is boiling!");
	}

	static void Main()
	{
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletTemperatureIRV2 tir =
		  new BrickletTemperatureIRV2(UID, ipcon); // Create device object

		ipcon.Connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Set emissivity to 0.98 (emissivity of water, 65535 * 0.98 = 64224.299)
		tir.SetEmissivity(64224);

		// Register object temperature reached callback to function ObjectTemperatureCB
		tir.ObjectTemperatureCallback += ObjectTemperatureCB;

		// Configure threshold for object temperature "greater than 100 °C" (unit is °C/10)
		// with a debounce period of 10s (10000ms)
		tir.SetObjectTemperatureCallbackConfiguration(10000, false, '>', 100*10, 0);

		Console.WriteLine("Press enter to exit");
		Console.ReadLine();
		ipcon.Disconnect();
	}
}
