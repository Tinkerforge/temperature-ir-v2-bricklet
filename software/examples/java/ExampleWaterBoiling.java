import com.tinkerforge.IPConnection;
import com.tinkerforge.BrickletTemperatureIRV2;

public class ExampleWaterBoiling {
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

		// Set emissivity to 0.98 (emissivity of water, 65535 * 0.98 = 64224.299)
		tir.setEmissivity(64224);

		// Add object temperature reached listener
		tir.addObjectTemperatureListener(new BrickletTemperatureIRV2.ObjectTemperatureListener() {
			public void objectTemperature(int temperature) {
				System.out.println("Object Temperature: " + temperature/10.0 + " °C");
				System.out.println("The water is boiling!");
			}
		});

		// Configure threshold for object temperature "greater than 100 °C"
		// with a debounce period of 10s (10000ms)
		tir.setObjectTemperatureCallbackConfiguration(10000, false, '>', 100*10, 0);

		System.out.println("Press key to exit"); System.in.read();
		ipcon.disconnect();
	}
}
