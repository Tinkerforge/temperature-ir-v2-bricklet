/* temperature-ir-v2-bricklet
 * Copyright (C) 2017 Olaf Lüke <olaf@tinkerforge.com>
 *
 * mlx90614.c: MLX90614 driver
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

#include "mlx90614.h"

#include "configs/config_mlx90614.h"

#include "bricklib2/logging/logging.h"

#define MLX90614_DATA_LENGTH      3
#define MLX90614_REG_EMISSIVITY   0x24            // read/write
#define MLX90614_REG_TA           0x6             // read-only
#define MLX90614_REG_TOBJ1        0x7             // read-only
#define MLX90614_TEMP_CRC_POLYNOM (0x1070U << 3)  // = X^8+X^2+X^1+X^0

MLX90614 mlx90614;

static uint8_t mlx90614_calculate_pec(const uint8_t *data, const uint8_t length) {
	uint16_t crc = 0;
	for(uint8_t i = 0; i < length; i++) {
		crc = ((crc >> 8) ^ data[i]) << 8;
		for(uint8_t j = 0; j < 8; j++) {
			if((crc & 0x8000) != 0) {
				crc = crc ^ MLX90614_TEMP_CRC_POLYNOM;
			}
			crc = crc << 1;
		}
	}

	return crc >> 8;
}

// Calculate 10th degree celsius
static int16_t mlx90614_value_to_celsius(const uint16_t value) {
	// TODO: Do we want to check that temperature range is between -710 and 3900?
	return value/5 - 2731;
}



void mlx90614_init(void) {
	memset(&mlx90614, 0, sizeof(MLX90614));

	mlx90614.emissivity_set = MLX90614_SET_EMISSIVITY_STATE_NONE;
	mlx90614.emissivity_get = true; // After initialization we always read the emissivity at first

	mlx90614.i2c_fifo.baudrate         = MLX90614_I2C_BAUDRATE;
	mlx90614.i2c_fifo.address          = MLX90614_I2C_ADDRESS;
	mlx90614.i2c_fifo.i2c              = MLX90614_I2C;

	mlx90614.i2c_fifo.scl_port         = MLX90614_SCL_PORT;
	mlx90614.i2c_fifo.scl_pin          = MLX90614_SCL_PIN;
	mlx90614.i2c_fifo.scl_mode         = MLX90614_SCL_PIN_MODE;
	mlx90614.i2c_fifo.scl_input        = MLX90614_SCL_INPUT;
	mlx90614.i2c_fifo.scl_source       = MLX90614_SCL_SOURCE;
	mlx90614.i2c_fifo.scl_fifo_size    = MLX90614_SCL_FIFO_SIZE;
	mlx90614.i2c_fifo.scl_fifo_pointer = MLX90614_SCL_FIFO_POINTER;

	mlx90614.i2c_fifo.sda_port         = MLX90614_SDA_PORT;
	mlx90614.i2c_fifo.sda_pin          = MLX90614_SDA_PIN;
	mlx90614.i2c_fifo.sda_mode         = MLX90614_SDA_PIN_MODE;
	mlx90614.i2c_fifo.sda_input        = MLX90614_SDA_INPUT;
	mlx90614.i2c_fifo.sda_source       = MLX90614_SDA_SOURCE;
	mlx90614.i2c_fifo.sda_fifo_size    = MLX90614_SDA_FIFO_SIZE;
	mlx90614.i2c_fifo.sda_fifo_pointer = MLX90614_SDA_FIFO_POINTER;

	i2c_fifo_init(&mlx90614.i2c_fifo);

	system_timer_sleep_ms(100);
}

void mlx90614_tick(void) {
	I2CFifoState state = i2c_fifo_next_state(&mlx90614.i2c_fifo);

	if(state & I2C_FIFO_STATE_ERROR) {
		loge("MLX90614 I2C error: %d\n\r", state);
		mlx90614_init();
		return;
	}

	switch(state) {
		case I2C_FIFO_STATE_READ_REGISTER_READY: {
			uint8_t buffer[16];
			uint8_t length = i2c_fifo_read_fifo(&mlx90614.i2c_fifo, buffer, 16);
			if(length != 3) {
				loge("MLX90614 I2C unexpected read length : %d\n\r", length);
				mlx90614_init();
				break;
			}

			uint8_t pec_data[5] = {0};
			pec_data[0] = MLX90614_I2C_ADDRESS << 1;
			switch(mlx90614.get_state) {
				case MLX90614_GET_STATE_EMISSIVITY: pec_data[1] = MLX90614_REG_EMISSIVITY;   break;
				case MLX90614_GET_STATE_AMBIENT:    pec_data[1] = MLX90614_REG_TA;           break;
				case MLX90614_GET_STATE_OBJECT:     pec_data[1] = MLX90614_REG_TOBJ1;        break;
				case MLX90614_GET_STATE_NONE:
				default: loge("MLX90614 unexpected get state : %u\n\r", mlx90614.get_state); return;
			}

			pec_data[2] = (MLX90614_I2C_ADDRESS << 1) | 1;
			pec_data[3] = buffer[0];
			pec_data[4] = buffer[1];
			uint8_t pec = mlx90614_calculate_pec(pec_data, 5);
			if(pec != buffer[2]) {
				return; // If checksum is not OK we try again
			}

			uint16_t value = buffer[0] | (buffer[1] << 8);
			switch(mlx90614.get_state) {
				case MLX90614_GET_STATE_EMISSIVITY: {
					// It seems like on initial boot-up the emissivity reads 0, when it is 0xFFFF.
					// Is this documented somewhere?
					mlx90614.emissivity = value == 0 ? 0xFFFF : value;
					mlx90614.emissivity_get = false;
					break;
				}

				case MLX90614_GET_STATE_AMBIENT: {
					mlx90614.temperature_ambient = value;
					mlx90614.read_ambient = false; // read object temperature next
					break;
				}

				case MLX90614_GET_STATE_OBJECT: {
					mlx90614.temperature_object = value;
					mlx90614.read_ambient = true; // read ambient temperature next
					break;
				}

				case MLX90614_GET_STATE_NONE:
				default: {
					loge("MLX90614 unexpected get state : %u\n\r", mlx90614.get_state);
					break;
				}
			}

			mlx90614.get_state = MLX90614_GET_STATE_NONE;

			break;
		}

		case I2C_FIFO_STATE_WRITE_REGISTER_READY: {
			// To write emissivity we have to first erase it (write 0) and can then write the emissivity we want to set
			switch(mlx90614.emissivity_set) {
				case MLX90614_SET_EMISSIVITY_STATE_ERASE: mlx90614.emissivity_set = MLX90614_SET_EMISSIVITY_STATE_WRITE; break;
				case MLX90614_SET_EMISSIVITY_STATE_WRITE: mlx90614.emissivity_set = MLX90614_SET_EMISSIVITY_STATE_NONE; break;
				default: loge("MLX90614 unrecognized emissivity state : %u\n\r", mlx90614.emissivity_set); mlx90614.emissivity_set = MLX90614_SET_EMISSIVITY_STATE_NONE; break;
			}

			break;
		}

		case I2C_FIFO_STATE_IDLE: {
			if(mlx90614.emissivity_get) {
				i2c_fifo_read_register(&mlx90614.i2c_fifo, MLX90614_REG_EMISSIVITY, MLX90614_DATA_LENGTH);
			} else if(mlx90614.emissivity_set != MLX90614_SET_EMISSIVITY_STATE_NONE) {
				// Check minimum value
				uint16_t emissivity = mlx90614.emissivity;
				if(emissivity < (0xFFFF/10 + 1)) {
					emissivity = (0xFFFF/10 + 1);
				}

				// Construct dummy packet to calculate pec
				uint8_t data[5];
				data[0] = MLX90614_I2C_ADDRESS << 1;
				data[1] = MLX90614_REG_EMISSIVITY;
				if(mlx90614.emissivity_set == MLX90614_SET_EMISSIVITY_STATE_ERASE) {
					data[2] = 0;
					data[3] = 0;
				} else {
					data[2] = emissivity        & 0xFF;
					data[3] = (emissivity >> 8) & 0xFF;
				}
				data[4] = mlx90614_calculate_pec(data, 4);

				i2c_fifo_write_register(&mlx90614.i2c_fifo, MLX90614_REG_EMISSIVITY, MLX90614_DATA_LENGTH, &data[2], true);
			} else if(mlx90614.read_ambient) {
				i2c_fifo_read_register(&mlx90614.i2c_fifo, MLX90614_REG_TA, MLX90614_DATA_LENGTH);
			} else {
				i2c_fifo_read_register(&mlx90614.i2c_fifo, MLX90614_REG_TOBJ1, MLX90614_DATA_LENGTH);
			}
			break;
		}

		default: {
			// If we end up in a ready state that we don't handle, something went wrong
			if(state & I2C_FIFO_STATE_READY) {
				loge("MLX90614 I2C unrecognized ready state : %d\n\r", state);
				mlx90614_init();
			}
			return;
		}
	}
}

int16_t mlx90614_get_ambient_temperature(void) {
	return mlx90614_value_to_celsius(mlx90614.temperature_ambient);
}

int16_t mlx90614_get_object_temperature(void) {
	return mlx90614_value_to_celsius(mlx90614.temperature_object);
}
