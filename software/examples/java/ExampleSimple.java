import com.tinkerforge.IPConnection;
import com.tinkerforge.BrickletTemperatureIRV2;

public class ExampleSimple {
	private static final String HOST = "localhost";
	private static final int PORT = 4223;

	// Change XYZ to the UID of your Temperature IR Bricklet 2.0
	private static final String UID = "XYZ";

	// Note: To make the example code cleaner we do not handle exceptions. Exceptions
	//       you might normally want to catch are described in the documentation
	public static void main(String args[]) throws Exception {
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletTemperatureIRV2 tir =
		  new BrickletTemperatureIRV2(UID, ipcon); // Create device object

		ipcon.connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Get current ambient temperature
		int ambientTemperature = tir.getAmbientTemperature(); // Can throw com.tinkerforge.TimeoutException
		System.out.println("Ambient Temperature: " + ambientTemperature/10.0 + " °C");

		// Get current object temperature
		int objectTemperature = tir.getObjectTemperature(); // Can throw com.tinkerforge.TimeoutException
		System.out.println("Object Temperature: " + objectTemperature/10.0 + " °C");

		System.out.println("Press key to exit"); System.in.read();
		ipcon.disconnect();
	}
}
