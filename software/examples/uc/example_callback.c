// This example is not self-contained.
// It requres usage of the example driver specific to your platform.
// See the HAL documentation.

#include "bindings/hal_common.h"
#include "bindings/bricklet_temperature_ir_v2.h"

#define UID "XYZ" // Change XYZ to the UID of your Temperature IR Bricklet 2.0

void check(int rc, const char* msg);

void example_setup(TF_HalContext *hal);
void example_loop(TF_HalContext *hal);


// Callback function for object temperature callback
static void object_temperature_handler(TF_TemperatureIRV2 *device, int16_t temperature,
                                       void *user_data) {
	(void)device; (void)user_data; // avoid unused parameter warning

	tf_hal_printf("Object Temperature: %d 1/%d °C\n", temperature, 10);
}

static TF_TemperatureIRV2 tir;

void example_setup(TF_HalContext *hal) {
	// Create device object
	check(tf_temperature_ir_v2_create(&tir, UID, hal), "create device object");

	// Register object temperature callback to function object_temperature_handler
	tf_temperature_ir_v2_register_object_temperature_callback(&tir,
	                                                          object_temperature_handler,
	                                                          NULL);

	// Set period for object temperature callback to 1s (1000ms) without a threshold
	tf_temperature_ir_v2_set_object_temperature_callback_configuration(&tir, 1000, false, 'x', 0, 0);
}

void example_loop(TF_HalContext *hal) {
	// Poll for callbacks
	tf_hal_callback_tick(hal, 0);
}
