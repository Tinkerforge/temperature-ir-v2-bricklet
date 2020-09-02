// This example is not self-contained.
// It requres usage of the example driver specific to your platform.
// See the HAL documentation.

#include "bindings/hal_common.h"
#include "bindings/bricklet_temperature_ir_v2.h"

#define UID "XYZ" // Change XYZ to the UID of your Temperature IR Bricklet 2.0

void check(int rc, const char* msg);

void example_setup(TF_HalContext *hal);
void example_loop(TF_HalContext *hal);


static TF_TemperatureIRV2 tir;

void example_setup(TF_HalContext *hal) {
	// Create device object
	check(tf_temperature_ir_v2_create(&tir, UID, hal), "create device object");

	// Get current ambient temperature
	int16_t ambient_temperature;
	check(tf_temperature_ir_v2_get_ambient_temperature(&tir,
	                                                   &ambient_temperature), "get ambient temperature");

	tf_hal_printf("Ambient Temperature: %d 1/%d °C\n", ambient_temperature, 10.0);

	// Get current object temperature
	int16_t object_temperature;
	check(tf_temperature_ir_v2_get_object_temperature(&tir,
	                                                  &object_temperature), "get object temperature");

	tf_hal_printf("Object Temperature: %d 1/%d °C\n", object_temperature, 10.0);
}

void example_loop(TF_HalContext *hal) {
	// Poll for callbacks
	tf_hal_callback_tick(hal, 0);
}
