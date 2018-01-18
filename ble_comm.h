#ifndef BLE_COMM_H__
#define BLE_COMM_H__

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "ble_nus.h"
#include "scale_setup.h"
#include "flash_storage.h"
#include "device_name.h"



void ble_debug(uint8_t * buffer);
extern uint8_t ble_string_put[10];


#endif
