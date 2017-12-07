using System;
using Tinkerforge;

class Example
{
	private static string HOST = "localhost";
	private static int PORT = 4223;
	private static string UID = "XYZ"; // Change XYZ to the UID of your Temperature IR Bricklet 2.0

	// Callback function for object temperature callback (parameter has unit °C/10)
	static void ObjectTemperatureCB(BrickletTemperatureIRV2 sender, short temperature)
	{
		Console.WriteLine("Object Temperature: " + temperature/10.0 + " °C");
	}

	static void Main()
	{
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletTemperatureIRV2 tir =
		  new BrickletTemperatureIRV2(UID, ipcon); // Create device object

		ipcon.Connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Register object temperature callback to function ObjectTemperatureCB
		tir.ObjectTemperatureCallback += ObjectTemperatureCB;

		// Set period for object temperature callback to 1s (1000ms) without a threshold
		tir.SetObjectTemperatureCallbackConfiguration(1000, false, 'x', 0, 0);

		Console.WriteLine("Press enter to exit");
		Console.ReadLine();
		ipcon.Disconnect();
	}
}
