#ifndef REMOTE_H__
#define REMOTE_H__

#include "nrf_drv_gpiote.h"
#include "correct.h"
#include "rgb_led.h"
#include "SEGGER_RTT.h"
#include "flash_storage.h"
#include "app_timer.h"
#include "remote_work.h"
#include "remote_setup.h"
#include "scale_setup.h"



#define REMOTE_433
//#define NEW_REMOTE
//#define OLD_REMOTE

#ifdef REMOTE_433

#define PIN_IN_1 14
#define PIN_IN_2 13
#define PIN_IN_3 11
#define PIN_IN_4 12

#endif

//test
#ifdef NEW_REMOTE

#define PIN_IN_1 11
#define PIN_IN_2 12
#define PIN_IN_3 13
#define PIN_IN_4 14

#endif

#ifdef OLD_REMOTE

#define PIN_IN_1 12
#define PIN_IN_2 13
#define PIN_IN_3 14
#define PIN_IN_4 15

#endif

#define WORK_MODE 								0
#define CORR_BUT_MODE 	  				1
#define CALL_MODE 								2
#define CHECK_SUB_DATA						3
#define CHANGE_SUB_DATA						4
#define FEEDBACK_SET_MODE 				5
#define CORR_SETUP_MODE	  				6  // auto correct setup
#define FACTORY_MODE							7
#define STATUS_DEFINE       			8
#define CORR_OFFSET_KG			 			9

#define CORRECT_COUNT_MAX 			3000
//#define CORRECT_COUNT_MAX_DEMO 	100

#define WORK_DAYS_MAX				    300
//#define WORK_DAYS_MAX_DEMO	    2

#define WORK_HOURS_MAX			WORK_DAYS_MAX*24



#define RESET_MAX						 1000
//#define RESET_MAX_DEMO			 20



#define CORRECT_COUNT_MAX_DEMO		150
#define CORRECT_COUNT_MAX_DEMO1		1000
#define CORRECT_COUNT_MAX_DEMO2		1500
#define CORRECT_COUNT_MAX_DEMO3		2000
#define CORRECT_COUNT_MAX_DEMO4		2500
#define CORRECT_COUNT_MAX_DEMO5		3000



#define WORK_HOURS_MAX_DEMO					4


#define WORK_DAYS_MAX_DEMO	    		 2
#define WORK_DAYS_MAX_DEMO1			   100
#define WORK_DAYS_MAX_DEMO2			   150
#define WORK_DAYS_MAX_DEMO3			   200
#define WORK_DAYS_MAX_DEMO4			   350
#define WORK_DAYS_MAX_DEMO5			   400

#define RESET_MAX_DEMO			 			 50
#define RESET_MAX_DEMO1			       1000
#define RESET_MAX_DEMO2			       1500
#define RESET_MAX_DEMO3			       2000
#define RESET_MAX_DEMO4			       2500
#define RESET_MAX_DEMO5			       3000




#define ACTIVATE_ATTEMPTS_MAX  20

#define TIME_TO_SLEEP 1800

extern uint8_t exp_subsriber;

extern uint32_t  cur_cor_max;
extern uint32_t  cur_hr_max;
extern uint32_t  cur_res_max;

extern uint16_t time_to_sleep;
extern uint8_t button_event;

extern uint8_t pin_in1_is_set;
extern uint8_t pin_in1_is_release;
extern uint8_t pin_in1_long_press;

extern uint8_t pin_in2_is_set;
extern uint8_t pin_in2_is_release;
extern uint8_t pin_in2_long_press;

extern uint8_t pin_in3_is_set;
extern uint8_t pin_in3_is_release;
extern uint8_t pin_in3_long_press;

extern uint8_t pin_in4_is_set;
extern uint8_t pin_in4_is_release;
extern uint8_t pin_in4_long_press;



extern uint8_t short_delay;

extern uint16_t time_to_sleep;

extern uint8_t push_count1;
extern uint8_t push_count2;
extern uint8_t push_count3;
extern uint8_t push_count4;

extern uint8_t remote_mode;

void set_up_mode_indicate(void);
void test_expired(void);
void start_timer_adc(void);
void nrf_gpiote(void);
void timer_remote_butts_init(void);
void flag_analize(void);
void reset_long_press_flags(void);
void reset_release_flags(void);
void start_timer_05s(void);
void start_timer_2s(void);
void start_timer_02s(void);
void stop_timer_02s(void);
void stop_timer(void);
void start_timer(uint16_t num);
#endif
