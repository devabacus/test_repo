#include "device_name.h"


uint8_t device_name[9] = DEVICE_NAME;
uint8_t activation_code[9] = "";

uint32_t demo;
uint32_t demo1 = 0;
uint32_t demo2 = 0;
uint32_t demo3 = 0;
uint32_t demo4 = 0;
uint32_t demo5 = 0;


//721113101
//121011101

void name_convert(void) {
	
	
	memcpy(activation_code, device_name, sizeof(device_name));
//	//activation_code[2] = '9';
	activation_code[0] = activation_code[8];
	activation_code[3] = activation_code[7];
	activation_code[6] = activation_code[2];
	activation_code[5] = activation_code[4];
	
	
	demo = atoi((char*)activation_code);
	demo1 = demo/4218 - 6219;  //22470
	demo2 = demo/7123 - 3113;  //13875
	demo3 = demo/5512 - 1231;  //20723
	demo4 = demo/3138 - 7131;	 //31432
	demo5 = demo/1921 - 1214;  //61779
	
	//ABD -С- ABD
	//473 -С- 210   Число 473210
	// 22     384

}


