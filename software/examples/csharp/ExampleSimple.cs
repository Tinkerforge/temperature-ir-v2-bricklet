using System;
using Tinkerforge;

class Example
{
	private static string HOST = "localhost";
	private static int PORT = 4223;
	private static string UID = "XYZ"; // Change XYZ to the UID of your Temperature IR Bricklet 2.0

	static void Main()
	{
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletTemperatureIRV2 tir =
		  new BrickletTemperatureIRV2(UID, ipcon); // Create device object

		ipcon.Connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Get current ambient temperature
		short ambientTemperature = tir.GetAmbientTemperature();
		Console.WriteLine("Ambient Temperature: " + ambientTemperature/10.0 + " °C");

		// Get current object temperature
		short objectTemperature = tir.GetObjectTemperature();
		Console.WriteLine("Object Temperature: " + objectTemperature/10.0 + " °C");

		Console.WriteLine("Press enter to exit");
		Console.ReadLine();
		ipcon.Disconnect();
	}
}
