/* temperature-ir-v2-bricklet
 * Copyright (C) 2017 Olaf Lüke <olaf@tinkerforge.com>
 *
 * mlx90614.h: MLX90614 driver
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

#ifndef MLX90614_H
#define MLX90614_H

#include <stdint.h>

#include "bricklib2/hal/i2c_fifo/i2c_fifo.h"


typedef struct {
	I2CFifo i2c_fifo;

	uint16_t temperature_ambient;
	uint16_t temperature_object;

	uint16_t emissivity;
	bool emissivity_new;

	uint32_t error_count;

	bool first;
} MLX90614;

void mlx90614_init(void);
void mlx90614_tick(void);
int16_t mlx90614_get_ambient_temperature(void);
int16_t mlx90614_get_object_temperature(void);

#endif
