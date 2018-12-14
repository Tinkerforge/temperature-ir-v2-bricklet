package main

import (
	"fmt"
	"tinkerforge/ipconnection"
	"tinkerforge/temperature_ir_v2_bricklet"
)

const ADDR string = "localhost:4223"
const UID string = "XYZ" // Change XYZ to the UID of your Temperature IR Bricklet 2.0.

func main() {
	ipcon := ipconnection.New()
	defer ipcon.Close()
	tir, _ := temperature_ir_v2_bricklet.New(UID, &ipcon) // Create device object.

	ipcon.Connect(ADDR) // Connect to brickd.
	defer ipcon.Disconnect()
	// Don't use device before ipcon is connected.

	// Set emissivity to 0.98 (emissivity of water, 65535 * 0.98 = 64224.299)
	tir.SetEmissivity(64224)

	tir.RegisterObjectTemperatureCallback(func(temperature int16) {
		fmt.Printf("Object Temperature: %d °C\n", float64(temperature)/10.0)
		fmt.Println("The water is boiling!")
	})

	// Configure threshold for object temperature "greater than 100 °C"
	// with a debounce period of 10s (10000ms).
	tir.SetObjectTemperatureCallbackConfiguration(10000, false, '>', 100*10, 0)

	fmt.Print("Press enter to exit.")
	fmt.Scanln()

}
