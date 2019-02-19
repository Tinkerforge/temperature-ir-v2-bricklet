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

#include "bricklib2/os/coop_task.h"
#include "bricklib2/hal/i2c_fifo/i2c_fifo.h"

#include "bricklib2/logging/logging.h"

#define MLX90614_DATA_LENGTH      3
#define MLX90614_REG_SLEEP        0xFF
#define MLX90614_REG_EMISSIVITY   0x24            // read/write
#define MLX90614_REG_CONFIG       0x25            // read/write
#define MLX90614_REG_TA           0x6             // read-only
#define MLX90614_REG_TOBJ1        0x7             // read-only
#define MLX90614_TEMP_CRC_POLYNOM (0x1070U << 3)  // = X^8+X^2+X^1+X^0

MLX90614 mlx90614;
CoopTask mlx90614_task;

XMC_GPIO_CONFIG_t mlx90614_pin_output_low = {
	.mode             = XMC_GPIO_MODE_OUTPUT_PUSH_PULL,
	.output_level     = XMC_GPIO_OUTPUT_LEVEL_LOW
};

XMC_GPIO_CONFIG_t mlx90614_pin_input = {
	.mode             = XMC_GPIO_MODE_INPUT_TRISTATE,
	.input_hysteresis = XMC_GPIO_INPUT_HYSTERESIS_LARGE
};

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

static bool mlx90614_check_pec_for_getter(const uint8_t data[3], const uint8_t reg) {
	uint8_t pec_data[5] = {
		MLX90614_I2C_ADDRESS << 1,
		reg,
		(MLX90614_I2C_ADDRESS << 1) | 1,
		data[0],
		data[1]
	};

	return mlx90614_calculate_pec(pec_data, 5) == data[2];
}

// Calculate 10th degree celsius
static int16_t mlx90614_value_to_celsius(const uint16_t value) {
	// TODO: Do we want to check that temperature range is between -710 and 3900?
	return value/5 - 2731;
}


void mlx90614_init_i2c(void) {
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
}

void mlx90614_task_update_emissivity(void) {
	// Construct dummy packet to calculate pec
	uint8_t data_pec[5] = {
		MLX90614_I2C_ADDRESS << 1,
		MLX90614_REG_EMISSIVITY,
		0, 0
	};

	// First erase emissivity (write 0)
	data_pec[4] = mlx90614_calculate_pec(data_pec, 4);
	i2c_fifo_coop_write_register(&mlx90614.i2c_fifo, MLX90614_REG_EMISSIVITY, MLX90614_DATA_LENGTH, &data_pec[2], true);

	// Wait 20ms beteween erase and write
	coop_task_sleep_ms(20);

	// then write the emissivity as set by the user
	data_pec[2] = mlx90614.emissivity        & 0xFF;
	data_pec[3] = (mlx90614.emissivity >> 8) & 0xFF;
	data_pec[4] = mlx90614_calculate_pec(data_pec, 4);
	i2c_fifo_coop_write_register(&mlx90614.i2c_fifo, MLX90614_REG_EMISSIVITY, MLX90614_DATA_LENGTH, &data_pec[2], true);
}

void mlx90614_task_sleep(void) {
	uint8_t data_pec[3] = {
		MLX90614_I2C_ADDRESS << 1,
		MLX90614_REG_SLEEP
	};

	data_pec[2] = mlx90614_calculate_pec(data_pec, 2);
	i2c_fifo_coop_write_register(&mlx90614.i2c_fifo, MLX90614_REG_SLEEP, 1, &data_pec[2], true);
	XMC_GPIO_Init(MLX90614_SCL_PORT, MLX90614_SCL_PIN, &mlx90614_pin_output_low);
	XMC_GPIO_Init(MLX90614_SDA_PORT, MLX90614_SDA_PIN, &mlx90614_pin_input);
}

void mlx90614_task_wakeup(void) {
	XMC_GPIO_Init(MLX90614_SCL_PORT, MLX90614_SCL_PIN, &mlx90614_pin_input);
	XMC_GPIO_Init(MLX90614_SDA_PORT, MLX90614_SDA_PIN, &mlx90614_pin_output_low);
	coop_task_sleep_ms(40); // at least 33ms
	XMC_GPIO_Init(MLX90614_SDA_PORT, MLX90614_SDA_PIN, &mlx90614_pin_input);
	coop_task_sleep_ms(250);

	// PWM to SMBus mode
	XMC_GPIO_Init(MLX90614_SCL_PORT, MLX90614_SCL_PIN, &mlx90614_pin_output_low);
	coop_task_sleep_ms(3); // at least 1.44ms
	XMC_GPIO_Init(MLX90614_SCL_PORT, MLX90614_SCL_PIN, &mlx90614_pin_input);
	mlx90614_init_i2c();
}

void mlx90614_task_reset(void) {
	// The MLX90614 has no reset functionality.
	// Instead we bring it into sleep mode, wait a bit and wake it up again.
	mlx90614_task_sleep();
	coop_task_sleep_ms(500);
	mlx90614_task_wakeup();

	uint8_t dummy[MLX90614_DATA_LENGTH];
	i2c_fifo_coop_read_register(&mlx90614.i2c_fifo, MLX90614_REG_CONFIG, MLX90614_DATA_LENGTH, dummy);
}

void mlx90614_task_tick(void) {
	uint8_t data[MLX90614_DATA_LENGTH];

	// 250ms startup time
	coop_task_sleep_ms(250);

	i2c_fifo_coop_read_register(&mlx90614.i2c_fifo, MLX90614_REG_CONFIG, MLX90614_DATA_LENGTH, data);

	// Get emissivity
	i2c_fifo_coop_read_register(&mlx90614.i2c_fifo, MLX90614_REG_EMISSIVITY, MLX90614_DATA_LENGTH, data);
	if(mlx90614_check_pec_for_getter(data, MLX90614_REG_EMISSIVITY)) {
		const uint16_t value = data[0] | (data[1] << 8);
		mlx90614.emissivity = value == 0 ? 0xFFFF : value;
	}

	while(true) {
		coop_task_sleep_ms(50);

		if(mlx90614.emissivity_new) {
			mlx90614.emissivity_new = false;
			mlx90614_task_update_emissivity();

			// We throw away the first value after emissivity change.
			// The first value sometimes is completely off.
			mlx90614.first = true;

			continue;
		}

		i2c_fifo_coop_read_register(&mlx90614.i2c_fifo, MLX90614_REG_TA, MLX90614_DATA_LENGTH, data);
		if(mlx90614_check_pec_for_getter(data, MLX90614_REG_TA)) {
			int16_t value = data[0] | (data[1] << 8);
			if(value & 0x8000) {
				// In case of error we try to re-set the emissivity up to 5 times
				mlx90614.error_count++;
				if(mlx90614.error_count > 5) {
					// After 5 errors we reset the MLX90614
					mlx90614_task_reset();

					mlx90614.error_count = 0;
				}
				mlx90614.emissivity_new = true;
				continue;
			} else if(!mlx90614.first) {
				mlx90614.temperature_ambient = value;
				mlx90614.error_count = 0;
			}
		}

		coop_task_sleep_ms(50);
		i2c_fifo_coop_read_register(&mlx90614.i2c_fifo, MLX90614_REG_TOBJ1, MLX90614_DATA_LENGTH, data);
		if(mlx90614_check_pec_for_getter(data, MLX90614_REG_TOBJ1)) {
			int16_t value = data[0] | (data[1] << 8);
			if(value & 0x8000) {
				// In case of error we try to re-set the emissivity up to 5 times
				mlx90614.error_count++;
				if(mlx90614.error_count > 5) {
					// After 5 errors we reset the MLX90614
					mlx90614_task_reset();

					mlx90614.error_count = 0;
				}
				mlx90614.emissivity_new = true;
				continue;
			} else if(!mlx90614.first) {
				mlx90614.temperature_object = value;
				mlx90614.error_count = 0;
			}
		}
		
		mlx90614.first = false;
	}
}

void mlx90614_init(void) {
	memset(&mlx90614, 0, sizeof(MLX90614));
	mlx90614_init_i2c();

	coop_task_init(&mlx90614_task, mlx90614_task_tick);
}

void mlx90614_tick(void) {
	coop_task_tick(&mlx90614_task);
}

int16_t mlx90614_get_ambient_temperature(void) {
	return mlx90614_value_to_celsius(mlx90614.temperature_ambient);
}

int16_t mlx90614_get_object_temperature(void) {
	return mlx90614_value_to_celsius(mlx90614.temperature_object);
}
