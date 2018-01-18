#ifndef DEVICE_NAME_H__
#define DEVICE_NAME_H__

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "ble_nus.h"
#include "scale_setup.h"
#include "flash_storage.h"

#define DEVICE_NAME                     "704123101"                         /**< Name of device. Will be included in the advertising data. */


//728123108
//721113101








extern uint8_t device_name[9];
extern uint8_t activation_code[9];

extern uint32_t demo1;
extern uint32_t demo2;
extern uint32_t demo3;
extern uint32_t demo4;
extern uint32_t demo5;





void name_convert(void);

#endif
