/* temperature-ir-v2-bricklet
 * Copyright (C) 2017 Olaf Lüke <olaf@tinkerforge.com>
 *
 * communication.h: TFP protocol message handling
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

#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <stdint.h>
#include <stdbool.h>

#include "bricklib2/protocols/tfp/tfp.h"
#include "bricklib2/bootloader/bootloader.h"

// Default functions
BootloaderHandleMessageResponse handle_message(const void *data, void *response);
void communication_tick(void);
void communication_init(void);

// Constants
#define TEMPERATURE_IR_V2_THRESHOLD_OPTION_OFF 'x'
#define TEMPERATURE_IR_V2_THRESHOLD_OPTION_OUTSIDE 'o'
#define TEMPERATURE_IR_V2_THRESHOLD_OPTION_INSIDE 'i'
#define TEMPERATURE_IR_V2_THRESHOLD_OPTION_SMALLER '<'
#define TEMPERATURE_IR_V2_THRESHOLD_OPTION_GREATER '>'

#define TEMPERATURE_IR_V2_BOOTLOADER_MODE_BOOTLOADER 0
#define TEMPERATURE_IR_V2_BOOTLOADER_MODE_FIRMWARE 1
#define TEMPERATURE_IR_V2_BOOTLOADER_MODE_BOOTLOADER_WAIT_FOR_REBOOT 2
#define TEMPERATURE_IR_V2_BOOTLOADER_MODE_FIRMWARE_WAIT_FOR_REBOOT 3
#define TEMPERATURE_IR_V2_BOOTLOADER_MODE_FIRMWARE_WAIT_FOR_ERASE_AND_REBOOT 4

#define TEMPERATURE_IR_V2_BOOTLOADER_STATUS_OK 0
#define TEMPERATURE_IR_V2_BOOTLOADER_STATUS_INVALID_MODE 1
#define TEMPERATURE_IR_V2_BOOTLOADER_STATUS_NO_CHANGE 2
#define TEMPERATURE_IR_V2_BOOTLOADER_STATUS_ENTRY_FUNCTION_NOT_PRESENT 3
#define TEMPERATURE_IR_V2_BOOTLOADER_STATUS_DEVICE_IDENTIFIER_INCORRECT 4
#define TEMPERATURE_IR_V2_BOOTLOADER_STATUS_CRC_MISMATCH 5

#define TEMPERATURE_IR_V2_STATUS_LED_CONFIG_OFF 0
#define TEMPERATURE_IR_V2_STATUS_LED_CONFIG_ON 1
#define TEMPERATURE_IR_V2_STATUS_LED_CONFIG_SHOW_HEARTBEAT 2
#define TEMPERATURE_IR_V2_STATUS_LED_CONFIG_SHOW_STATUS 3

// Function and callback IDs and structs
#define FID_GET_AMBIENT_TEMPERATURE 1
#define FID_SET_AMBIENT_TEMPERATURE_CALLBACK_CONFIGURATION 2
#define FID_GET_AMBIENT_TEMPERATURE_CALLBACK_CONFIGURATION 3
#define FID_GET_OBJECT_TEMPERATURE 5
#define FID_SET_OBJECT_TEMPERATURE_CALLBACK_CONFIGURATION 6
#define FID_GET_OBJECT_TEMPERATURE_CALLBACK_CONFIGURATION 7
#define FID_SET_EMISSIVITY 9
#define FID_GET_EMISSIVITY 10

#define FID_CALLBACK_AMBIENT_TEMPERATURE 4
#define FID_CALLBACK_OBJECT_TEMPERATURE 8

typedef struct {
	TFPMessageHeader header;
	uint16_t emissivity;
} __attribute__((__packed__)) SetEmissivity;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetEmissivity;

typedef struct {
	TFPMessageHeader header;
	uint16_t emissivity;
} __attribute__((__packed__)) GetEmissivity_Response;


// Function prototypes
BootloaderHandleMessageResponse set_emissivity(const SetEmissivity *data);
BootloaderHandleMessageResponse get_emissivity(const GetEmissivity *data, GetEmissivity_Response *response);

// Callbacks
bool handle_ambient_temperature_callback(void);
bool handle_object_temperature_callback(void);

#define COMMUNICATION_CALLBACK_TICK_WAIT_MS 1
#define COMMUNICATION_CALLBACK_HANDLER_NUM 2
#define COMMUNICATION_CALLBACK_LIST_INIT \
	handle_ambient_temperature_callback, \
	handle_object_temperature_callback, \


#endif
