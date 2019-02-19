/* temperature-ir-v2-bricklet
 * Copyright (C) 2017 Olaf Lüke <olaf@tinkerforge.com>
 *
 * communication.c: TFP protocol message handling
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include "communication.h"

#include "bricklib2/utility/communication_callback.h"
#include "bricklib2/protocols/tfp/tfp.h"
#include "bricklib2/utility/callback_value.h"

#include "mlx90614.h"

extern MLX90614 mlx90614;

CallbackValue_int16_t callback_value_object_temperature;
CallbackValue_int16_t callback_value_ambient_temperature;

BootloaderHandleMessageResponse handle_message(const void *message, void *response) {
	switch(tfp_get_fid_from_message(message)) {
		case FID_GET_AMBIENT_TEMPERATURE: return get_callback_value_int16_t(message, response, &callback_value_ambient_temperature);
		case FID_SET_AMBIENT_TEMPERATURE_CALLBACK_CONFIGURATION: return set_callback_value_callback_configuration_int16_t(message, &callback_value_ambient_temperature);
		case FID_GET_AMBIENT_TEMPERATURE_CALLBACK_CONFIGURATION: return get_callback_value_callback_configuration_int16_t(message, response, &callback_value_ambient_temperature);
		case FID_GET_OBJECT_TEMPERATURE: return get_callback_value_int16_t(message, response, &callback_value_object_temperature);
		case FID_SET_OBJECT_TEMPERATURE_CALLBACK_CONFIGURATION: return set_callback_value_callback_configuration_int16_t(message, &callback_value_object_temperature);
		case FID_GET_OBJECT_TEMPERATURE_CALLBACK_CONFIGURATION: return get_callback_value_callback_configuration_int16_t(message, response, &callback_value_object_temperature);
		case FID_SET_EMISSIVITY: return set_emissivity(message);
		case FID_GET_EMISSIVITY: return get_emissivity(message, response);
		default: return HANDLE_MESSAGE_RESPONSE_NOT_SUPPORTED;
	}
}

BootloaderHandleMessageResponse set_emissivity(const SetEmissivity *data) {
	mlx90614.emissivity = data->emissivity;
	if(mlx90614.emissivity < (0xFFFF/10 + 1)) {
		mlx90614.emissivity = 0xFFFF/10 + 1;
	}

	mlx90614.emissivity_new = true;

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_emissivity(const GetEmissivity *data, GetEmissivity_Response *response) {
	response->header.length = sizeof(GetEmissivity_Response);
	response->emissivity    = mlx90614.emissivity;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

bool handle_ambient_temperature_callback(void) {
	return handle_callback_value_callback_int16_t(&callback_value_ambient_temperature, FID_CALLBACK_AMBIENT_TEMPERATURE);
}

bool handle_object_temperature_callback(void) {
	return handle_callback_value_callback_int16_t(&callback_value_object_temperature, FID_CALLBACK_OBJECT_TEMPERATURE);
}

void communication_tick(void) {
	communication_callback_tick();
}

void communication_init(void) {
	callback_value_init_int16_t(&callback_value_object_temperature, mlx90614_get_object_temperature);
	callback_value_init_int16_t(&callback_value_ambient_temperature, mlx90614_get_ambient_temperature);

	communication_callback_init();
}
