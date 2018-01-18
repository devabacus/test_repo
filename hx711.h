#ifndef HX711_H__
#define HX711_H__


#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "nrf_delay.h"
#include "app_timer.h"
#include "nrf_drv_pwm.h"

//#define NEW_HX
//#define OLD_HX
#define LAST_HX

#ifdef NEW_HX
#define HX_DATA	27
#define HX_SCK	28
#endif

#ifdef OLD_HX

#define HX_DATA	28
#define HX_SCK	29
#endif

#ifdef LAST_HX
#define HX_DATA	29
#define HX_SCK	30
#endif


extern uint32_t adc_value;
extern char buffer[24];
static uint8_t adc_ready;
extern uint8_t adc_value_ready;

void HX711_init(void);
uint32_t Weighing(void);
void ConvertADC(void);

#endif
