// This example is not self-contained.
// It requres usage of the example driver specific to your platform.
// See the HAL documentation.

#include "bindings/hal_common.h"
#include "bindings/bricklet_temperature_ir_v2.h"

#define UID "XYZ" // Change XYZ to the UID of your Temperature IR Bricklet 2.0

void check(int rc, const char* msg);

void example_setup(TF_HAL *hal);
void example_loop(TF_HAL *hal);


// Callback function for object temperature reached callback
static void object_temperature_handler(TF_TemperatureIRV2 *device, int16_t temperature,
                                       void *user_data) {
	(void)device; (void)user_data; // avoid unused parameter warning

	tf_hal_printf("Object Temperature: %d 1/%d °C\n", temperature, 10);
	tf_hal_printf("The water is boiling!\n");
}

static TF_TemperatureIRV2 tir;

void example_setup(TF_HAL *hal) {
	// Create device object
	check(tf_temperature_ir_v2_create(&tir, UID, hal), "create device object");

	// Set emissivity to 0.98 (emissivity of water, 65535 * 0.98 = 64224.299)
	check(tf_temperature_ir_v2_set_emissivity(&tir, 64224), "call set_emissivity");

	// Register object temperature reached callback to function object_temperature_handler
	tf_temperature_ir_v2_register_object_temperature_callback(&tir,
	                                                          object_temperature_handler,
	                                                          NULL);

	// Configure threshold for object temperature "greater than 100 °C"
	// with a debounce period of 10s (10000ms)
	tf_temperature_ir_v2_set_object_temperature_callback_configuration(&tir, 10000, false, '>', 100*10, 0);
}

void example_loop(TF_HAL *hal) {
	// Poll for callbacks
	tf_hal_callback_tick(hal, 0);
}
