#include "scale_setup.h"


uint32_t cal_zero_value = 0;
uint32_t cal_load_value = 0;
uint32_t cal_turn_on = 0;
uint8_t  entry_to_cal = 0;
uint8_t  start_average_adc = 0;
uint8_t  count_average_adc = 0;
uint32_t average_adc = 0;
uint32_t adc_array[AVERAGE_ADC_TIMES];

uint8_t  scale_feedback = 0;
uint32_t cal_coef = 0;
uint32_t activate_code = 313;
//uint32_t hard_reset_code = 131;
uint32_t test_activate_code = 0;

uint32_t reset_attempts_code = 3;
uint32_t test_reset_attempts_code = 0;

uint8_t  scale_type = SCALE_600;

uint32_t adc_need = 0;
uint32_t cor_feedback = COR_PLUS_1;
uint32_t time_feedback = TIME_FEEDBACK;
uint32_t feedback = 0;
uint16_t clock_counter = 0;

uint8_t round_input = 1;
uint8_t test_activate_status = 0;

uint8_t counter_for_set_mode = 0;

uint8_t buf_check_sub_data[11] = "";

uint8_t  admin 							= 0;
uint8_t  root								= 0;
uint32_t admin_pass 				= 0;
uint32_t full_pass   				= 0;   
			
uint8_t  thousands						= 0;
uint8_t  hundreds 						= 0;
uint8_t  tens		 				  		= 0;
uint8_t  ones		 							= 0;
uint32_t current_life_counter = 0;

void generate_admin_pass(void)
{
	admin_pass = demo5+power_down_count+life_counter/60; 
	full_pass = (admin_pass + 158)/2;
	
	SEGGER_RTT_printf(0, "admin pass = %d\n\r", admin_pass);
}

void number_indicate(uint32_t number)
{	
	SEGGER_RTT_printf(0, "number for check = %d\n\r", number);
	
	thousands = number/1000;     															//0
		
	SEGGER_RTT_printf(0, "1000 - %d\n\r", thousands);
	
	hundreds	= (number - thousands*1000)/100;   									//1
	
	SEGGER_RTT_printf(0, "100 - %d\n\r", hundreds);
	
	tens 			= (number - thousands*1000 - hundreds*100)/10;   				
	SEGGER_RTT_printf(0, "10 - %d\n\r", tens);
	
	ones 			= number - thousands*1000 - hundreds*100 - tens*10;   				
	SEGGER_RTT_printf(0, "1 - %d\n\r", ones);	
	
	
		if(thousands>0)
		{
			rgb_set(0,0,50, thousands,1500);
			nrf_delay_ms(thousands*1000);
		}
		
		if(hundreds)
		{
			rgb_set(0,50,0, hundreds,1500);
			nrf_delay_ms(hundreds*1000);
		}
		
		if(tens)
		{
			rgb_set(50,0,0, tens,1500);
			nrf_delay_ms(tens*1000);
		}
		
		if(ones)
		{
			rgb_set(50,50,50, ones,1500);
		}
		
	
}

void fail_attempt(void)
{
		test_activate_status = 0;
		activate_attempts++;
		fds_update_value(&activate_attempts, file_id_c, fds_rk_activate_attempts);
		SEGGER_RTT_printf(0, "activate_attempts = %d\r\n", activate_attempts);
		test_activate_code = 0;
		
		SEGGER_RTT_printf(0, "FAIL\r\n");
		rgb_set(50, 0, 0, 1, 3000);
		round_input	= 1;
}


void start_led(void)
	{
		if(activate_status == 1)
			{
				rgb_set(0, 50, 0, 1, 1500);				
			}
			
		else if (activate_status > 1)
			{
				rgb_set(0, 50, 0, 2, 1500);				
			}
		else 
			{
				rgb_set(50, 0, 0, 1, 1500);
			}
			
		if(exp_subsriber)
		{
			nrf_delay_ms(1000);
			rgb_set(0, 0, 50, exp_subsriber, 1000);
		}
		
	}

void set_weight(uint16_t weight_value)
{
	adc_need = weight_value*((cal_load_value - cal_zero_value)/20)+cal_zero_value; 
	start_timer_adc();
	SEGGER_RTT_printf(0, "adc_need = %d\r\n", adc_need);
}

void sort_array(uint32_t* array, uint8_t size)
{
	for(int i = size-1; i>=0; i--)
		{
			for (int j=0; j<i; j++)
				{
					if(array[j] > array[j+1])
						{
							uint32_t tmp = array[j];
							array[j] = array[j+1];
							array[j+1] = tmp;
							//nrf_delay_ms(100);
						}
				}
		}
}


void find_average_in_array(uint32_t* array, uint8_t size)
{
				uint8_t start_id = NUM_EXCEED_MEMBERS/2;
				uint8_t end_id = size - start_id;
				//uint32_t average_adc_arr = 0;
			
				for(uint8_t i = start_id; i < end_id; i++)
					{
						
						average_adc = average_adc + adc_value;
					//	SEGGER_RTT_printf(0, "arr[%d] %d\n\r",i, array[i]);
					//	nrf_delay_ms(100);
						
					}
					
					average_adc = average_adc/(size - NUM_EXCEED_MEMBERS);
					
		}


void print_array(uint32_t* array, uint8_t size)
{
		for(int i = 0; i<size; i++)
			{
					SEGGER_RTT_printf(0, "arr[%d] %d\n\r",i, array[i]);
					nrf_delay_ms(100);
			}
	}


void find_average_adc(void)
{
			  rgb_set(50,0,0,0,1000);
				adc_array[count_average_adc] = adc_value;
				count_average_adc++;
	
				if(count_average_adc == AVERAGE_ADC_TIMES)
						{
							
							sort_array(adc_array, AVERAGE_ADC_TIMES);
							find_average_in_array(adc_array, AVERAGE_ADC_TIMES);
							
							switch(start_average_adc)
							{
								case 1:
									cal_zero_value = average_adc;
									fds_update_value(&cal_zero_value, file_id, fds_rk_cal_zero);
									SEGGER_RTT_printf(0, "zero - %d\n\r", cal_zero_value);
								break;
								
								case 2:
									cal_load_value = average_adc;
									if((scale_type == SCALE_600) && cal_zero_value)
										{
											 // adc value for one discrete //10 
											cal_coef = (cal_load_value - cal_zero_value)/NUM_OF_DISCRETE_FOR_CAL; 
											SEGGER_RTT_printf(0, "cal_coef - %d\n\r", cal_coef);
										}
										fds_update_value(&cal_coef, file_id, fds_rk_cal_zero+1);
										SEGGER_RTT_printf(0, "load - %d\n\r", cal_load_value);
										
								break;
								
								case 3:
									cal_turn_on = average_adc;
									fds_update_value(&cal_turn_on, file_id, fds_rk_cal_zero+2);
									SEGGER_RTT_printf(0, "turn_on - %d\n\r", cal_turn_on);
								
								break;
								
								}
								count_average_adc = 0;
								start_average_adc = 0;
								average_adc = 0;
								stop_timer_02s();
								
								
								rgb_set(0,0,0,0,0);
								
								//SEGGER_RTT_printf(0, "============\n\r");
								//SEGGER_RTT_printf(0, "%d\n\r", cal_zero_value);
								
							//	print_array(adc_array, AVERAGE_ADC_TIMES);
								//SEGGER_RTT_printf(0, "============\n\r");
								
								//print_array(adc_array, AVERAGE_ADC_TIMES);
								//SEGGER_RTT_printf(0, "============\n\r");
								
						}
					
				

}



void cal_unload(void)
{
		start_average_adc = 1;
	  start_timer_02s();
	}

void cal_load(void) // define adc_value for 10 discretes 
{
	start_average_adc = 2;
	start_timer_02s();
}

void define_corr_on(void)
{
	start_average_adc = 3;
	start_timer_02s();
	
}


void init_cal_values(void)
{
		fds_get_data(&cal_zero_value, file_id, fds_rk_cal_zero);
		fds_get_data(&cal_coef, file_id, fds_rk_cal_zero+1);
		fds_get_data(&cal_turn_on, file_id, fds_rk_cal_zero+2);
		
}



void delete_fds_c(void)
{
	
	for(uint16_t i = 0x0001; i<=0x000F; i++)
						{
						fds_test_find_and_delete(i, file_id_c);
						}
		rgb_set(50, 0, 0, 1, 3000);
}


void delete_fds(void)
{
	
	for(uint16_t i = 0x0010; i<=0x0021; i++)
						{
						fds_test_find_and_delete(i, file_id);
						}
	rgb_set(0, 50, 0, 1, 3000);
}




void scale_setup(void)
{
  	if(remote_mode != WORK_MODE && pin_in4_is_release && (corr_mode_button == 0))
				{
								remote_mode = WORK_MODE;
							//	offset_perc_state = STANDBY;
								test_activate_code = 0;
								round_input = 1;  // для ввода пароля активации первые три символа из шести
								rgb_set(50, 50, 50, 2, 1000);
						}

	
	  if((remote_mode == CALL_MODE) && !short_delay)
	    	{
				
				if(pin_in1_is_release)
					{
						if(adc_value > 1000)
						{
							cal_unload();		
							rgb_set(50, 0, 0, 1, 1000);
						}
						
					}
					
					
					if(pin_in1_long_press)
				{
					perc_offset_init_auto();		// in separate file
					rgb_set(0, 0, 50, 3, 300);
				}
					
					
				else if (pin_in2_is_release)	
					{
						cal_load();
						rgb_set(0, 50, 0, 1, 1000);
						
						//delete_fds();
						//delete_fds_c();
						//rgb_set(50, 50, 50, 2, 3000);
						
					}
				
				else if (pin_in3_is_release)
					{
						if((adc_value - cal_zero_value)>1000)
						{
							define_corr_on();
							rgb_set(0, 0, 50, 1, 1000);
							//remote_mode = WORK_MODE;
						}
					}
					
				else if (pin_in4_long_press)
						{
							{
									remote_mode = CHECK_SUB_DATA;
									rgb_set(50, 0, 0, 1, 3000);
									nrf_delay_ms(500);
									rgb_set(0, 50, 0, 1, 500);
									SEGGER_RTT_printf(0, "CHECK_SUB_DATA\n\r");
								  adc_value = 0;   // for not showing adc besides call mode
							}
							
							// save data to flash
						}
		}
				
		else if (remote_mode == CHECK_SUB_DATA)
				{
					adc_value = 0;
					
					if(pin_in1_long_press)
					{
  					number_indicate(cur_cor_max);				
						//SEGGER_RTT_printf(0, "long1\r\n");						
						
					}
					
					else if(pin_in2_long_press)
					{
						//SEGGER_RTT_printf(0, "long2\r\n");						
						number_indicate(cur_res_max);												
					}
					
					else if(pin_in3_long_press)
					{
						//SEGGER_RTT_printf(0, "long3\r\n");						
						number_indicate(cur_hr_max);
					}
					
					
					else if(pin_in1_is_release)
					{
  					number_indicate(corr_counter);														
					}
					
					else if(pin_in2_is_release)
					{
						number_indicate(power_down_count);				
						current_life_counter = life_counter;	
					}
					
					else if(pin_in3_is_release)
					{
						number_indicate(life_counter/60);
					}
					


					else if (pin_in4_long_press)
					
					{
						remote_mode = STATUS_DEFINE;
						rgb_set(0, 0, 50, 1, 3000);
						current_life_counter = life_counter;
						generate_admin_pass();
						SEGGER_RTT_printf(0, "current_life_counter = %d\n\r", current_life_counter);
						SEGGER_RTT_printf(0, "STATUS_DEFINE\r\n");
						
						
						
					}
					
					//return_to_work_mode();
				}

		else if (remote_mode == STATUS_DEFINE)
	  		{
				if(pin_in1_long_press)
				{
					rgb_set(50, 0, 0, activate_status, 1500);
				}
				else if(pin_in1_is_release)
				{
					if(round_input == 1)
					{
							test_activate_code += 100000;
					}
					else 
						{
							test_activate_code += 100;
						}
					rgb_set(0, 0, 50, 1, 500);
					SEGGER_RTT_printf(0, "%d\r\n", test_activate_code);
					nrf_delay_ms(300);
				}
				
				else if(pin_in2_is_release)
				{
					if(round_input == 1)
					{
							test_activate_code += 10000;
					}
					else 
						{
							test_activate_code += 10;
						}
					rgb_set(0, 50, 0, 1, 500);
					SEGGER_RTT_printf(0, "%d\r\n", test_activate_code);
					nrf_delay_ms(300);
				}
				
				else if(pin_in3_is_release)
				{
					if(round_input == 1)
					{
							test_activate_code += 1000;
					}
					else 
						{
							test_activate_code += 1;
						}
					rgb_set(50, 0, 0, 1, 500);
					SEGGER_RTT_printf(0, "%d\r\n", test_activate_code);
					pin_in3_is_release = 0;
					nrf_delay_ms(300);
				}
				else if (pin_in4_long_press)
				{
					
					if(round_input == 1)
					{
						round_input = 2;
						rgb_set(0, 0, 50, 1, 3000);
					
					}
					
					else if (round_input == 2)
					{
							
						if(test_activate_code == demo1)
							{
								test_activate_status = DEMO1; // check activation_status
								cur_cor_max = CORRECT_COUNT_MAX_DEMO1;
								cur_hr_max = WORK_DAYS_MAX_DEMO1*24;
								cur_res_max = RESET_MAX_DEMO1;
							}
						else if (test_activate_code == demo2) // check activation_code
							{
								test_activate_status = DEMO2;
								cur_cor_max = CORRECT_COUNT_MAX_DEMO2;
								cur_hr_max = WORK_DAYS_MAX_DEMO2*24;
								cur_res_max = RESET_MAX_DEMO2;
							}
						else if (test_activate_code == demo3)
							{
								test_activate_status = DEMO3;
								cur_cor_max = CORRECT_COUNT_MAX_DEMO3;
								cur_hr_max = WORK_DAYS_MAX_DEMO3*24;
								cur_res_max = RESET_MAX_DEMO3;
							}
						else if (test_activate_code == demo4)
							{
								test_activate_status = DEMO4;
								cur_cor_max = CORRECT_COUNT_MAX_DEMO4;
								cur_hr_max = WORK_DAYS_MAX_DEMO4*24;
								cur_res_max = RESET_MAX_DEMO4;
							}
						else if (test_activate_code == demo5)
							{
								test_activate_status = DEMO5;
								cur_cor_max = CORRECT_COUNT_MAX_DEMO5;
								cur_hr_max = WORK_DAYS_MAX_DEMO5*24;
								cur_res_max = RESET_MAX_DEMO5;
							}
							
						else if (test_activate_code == admin_pass)
							{
								test_activate_status = ADMIN;
								admin = 1;
								SEGGER_RTT_printf(0, "admin activated\r\n");
								
															
							}
							
							
						else if (test_activate_code == full_pass)
							{
								test_activate_status = ROOT;
								root = 1;
								SEGGER_RTT_printf(0, "root activated\r\n");
								
															
							}
							
						
							if(test_activate_code > 0)
							
							{
							
									if((test_activate_status > activate_status) && (activate_attempts < ACTIVATE_ATTEMPTS_MAX))
											{
										
													rgb_set(0, 50, 0, 1, 3000);	
													
													if(admin)
													{
														nrf_delay_ms(1000);
														rgb_set(0, 0, 50, 1, 1000);	
													}
													
													else if (root)
													{
														nrf_delay_ms(1000);
														rgb_set(50, 0, 0, 1, 1000);	
													}
												
													else
													{
														activate_status = test_activate_status;
														SEGGER_RTT_printf(0, "status = %d, code = %d\n\r", activate_status, test_activate_code);
														fds_update_value(&activate_status, file_id_c, fds_rk_activate_status);
														fds_update_value(&cur_cor_max, file_id_c, fds_rk_cur_cor_max);
														fds_update_value(&cur_hr_max, file_id_c, fds_rk_cur_cor_max+1);
														fds_update_value(&cur_res_max, file_id_c, fds_rk_cur_cor_max+2);	
													}
													
														SEGGER_RTT_printf(0, "ACTIVATED\r\n");
														test_activate_code = 0;
														test_activate_status = 0;
														round_input	= 1;
											}
																										
										else 
											{
												fail_attempt();
											}
							}
							
							else {
									rgb_set(0, 50, 0, 1, 2000);	
									nrf_delay_ms(1000);
									rgb_set(50, 0, 0, 1, 1000);	
							}
							
							  
								
								remote_mode = CHANGE_SUB_DATA;
								SEGGER_RTT_printf(0, "CHANGE_SUB_DATA\r\n");
						  }
					}
				}
				
		else if (remote_mode == CHANGE_SUB_DATA)
				{
					if(admin)
					{
							if(pin_in1_is_release)
									{
										cur_cor_max+=100;
										SEGGER_RTT_printf(0, "cur_cor_max = %d\r\n", cur_cor_max);
										rgb_set(0, 50, 0, 1, 1000);
									}
							
							else if(pin_in2_is_release)
									{
										cur_res_max+=100;
										SEGGER_RTT_printf(0, "cur_res_max = %d\r\n", cur_res_max);
										rgb_set(0, 50, 0, 1, 1000);
									}
							
							else if(pin_in3_is_release)
									{
										cur_hr_max+=100;
										SEGGER_RTT_printf(0, "cur_hr_max = %d\r\n", cur_hr_max);
										rgb_set(0, 50, 0, 1, 1000);
									}
									
							else if(pin_in1_long_press)
									{
										cur_cor_max+=1000;
										SEGGER_RTT_printf(0, "cur_cor_max = %d\r\n", cur_cor_max);
										rgb_set(0, 0, 50, 1, 1000);
									}
							
							else if(pin_in2_long_press)
									{
										cur_res_max+=1000;
										SEGGER_RTT_printf(0, "cur_res_max = %d\r\n", cur_res_max);
										rgb_set(0, 0, 50, 1, 1000);
									}
							
							else if(pin_in3_long_press)
									{
										cur_hr_max+=1000;
										SEGGER_RTT_printf(0, "cur_hr_max = %d\r\n", cur_hr_max);
										rgb_set(0, 0, 50, 1, 1000);
									}
									
							
					}
					
					if (pin_in4_long_press)
					
					{
						if(admin)
						{
							fds_update_value(&cur_cor_max, file_id_c, fds_rk_cur_cor_max);
							fds_update_value(&cur_hr_max, file_id_c, fds_rk_cur_cor_max+1);
							fds_update_value(&cur_res_max, file_id_c, fds_rk_cur_cor_max+2);	
						}
						
						remote_mode = FEEDBACK_SET_MODE;
						rgb_set(0, 50, 0, 1, 3000);
						//activate_status = 0;
						SEGGER_RTT_printf(0, "FEEDBACK_SET_MODE\r\n");
					}
					
					//return_to_work_mode();
				}	



		else if (remote_mode == FEEDBACK_SET_MODE)
	    	{
					if(pin_in1_is_release)
					{
						cor_feedback++;
						rgb_set(50, 0, 0, 1, 500);
						correct_value(cor_feedback);
						
					}
					
					if(pin_in1_long_press)
					{
						time_feedback+=500;
						
						if(time_feedback > 3000)
						{
							time_feedback = 500;
						}
						
						rgb_set(0,0,50,time_feedback/500,1000);
						
					}
					
					else if (pin_in3_is_release)
					{
						cor_feedback--;
						rgb_set(50, 0, 0, 1, 500);
						correct_value(cor_feedback);
					}
					
					else if (pin_in2_long_press)
					{
						switch(feedback)
						{
							case 0:
								feedback = 1; // cor_feedback
								rgb_set(0, 50, 0, 1, 1000);
								break;
							
							case 1:
								feedback = 2; // corrects
								rgb_set(0, 50, 0, 2, 1000);
								break;
							
							case 2:
								feedback = 0;
								rgb_set(50, 50, 50, 1, 1000);
								break;
							
						}
						
						fds_update_value(&feedback, file_id, fds_rk_feedback);
						
						SEGGER_RTT_printf(0, "feedback = %d\r\n", feedback);
					}
					
					
					
					else if (pin_in3_long_press)
					{
						//scale_feedback = 1;
						if(num_cor_buts == 3)
						{
							num_cor_buts = 9;
							rgb_set(50,0,0,1,5000);
							nrf_delay_ms(2000);
							rgb_set(50,0,0,4,1000);
							SEGGER_RTT_printf(0, "but=9\r\n");
						}
						else if (num_cor_buts == 9)
						{
							num_cor_buts = 3;
							rgb_set(50,0,0,3,1000);
							SEGGER_RTT_printf(0, "but=3\r\n");
						}
						
						fds_update_value(&num_cor_buts, file_id, fds_rk_num_cor_but);
						
						
						
						//SEGGER_RTT_printf(0, "scale_feedback = 1\r\n");
					}
					
					else if (pin_in3_is_release)
					{
						set_weight(11);
					}
					
					else if (pin_in4_long_press)
					
					{
						fds_update_value(&cor_feedback, file_id, fds_rk_cor_feedback);
						fds_update_value(&time_feedback, file_id, fds_rk_time_feedback);
						remote_mode = FACTORY_MODE;
						rgb_set(0, 50, 0, 1, 3000);
						SEGGER_RTT_printf(0, "FACTORY_MODE\r\n");
					}
					
		}
		
		else if (remote_mode == FACTORY_MODE)
				{
					if(pin_in1_is_release)
					{
						//delete_fds();						
						rgb_set(50, 50, 50, 1, 3000);
					}
					
					if(pin_in1_long_press)
						
					{
						
					}
					
					if(pin_in2_long_press)
					{
//						delete_fds();
//						delete_fds_c();
						rgb_set(50, 50, 50, 2, 3000);
					}
					
					
					else if (pin_in2_is_release)
					{
					}
					else if (pin_in3_long_press)
					{
						
					}
					
					else if (pin_in4_long_press)
					
					{
						remote_mode = WORK_MODE;
						rgb_set(0, 0, 50, 1, 3000);
						SEGGER_RTT_printf(0, "WORK_MODE\r\n");
					}
					
					
					//return_to_work_mode();
					
				}
				
	
	

		else if (remote_mode == WORK_MODE)
				{
				
					 //SEGGER_RTT_printf(0, "I entry\r\n");
					if(pin_in1_is_set && pin_in4_long_press)
					
						{
							rgb_set(50,0,0,1,1000);
							nrf_delay_ms(200);
							rgb_set(0,50,0,1,1000);
							nrf_delay_ms(200);
							rgb_set(0,0,50,1,1000);
							remote_mode = CALL_MODE;
							time_to_sleep = TIME_TO_SLEEP;
							SEGGER_RTT_printf(0, "CAL MODE\r\n");
							short_delay = 0;
						}
						
					else if ((pin_in4_long_press) && (counter_for_set_mode < 5))
					{
						//remote_mode = CORR_SETUP_MODE;
						//counter_for_set_mode = 0;
						
					}
					
						if(pin_in4_is_release)
						{		
								if(!pin_in4_long_press)
								{
									counter_for_set_mode++;
//									SEGGER_RTT_printf(0, "counter_for_set_mode = %d \n\r", counter_for_set_mode);
								}
																
								else if (counter_for_set_mode > 5)
								{
									counter_for_set_mode = 0;
								}
						}
					
			  		if (pin_in4_long_press)
						{
							if(counter_for_set_mode == 3)
							{
									SEGGER_RTT_printf(0, "COR_AUTO_MODE\r\n");
							if(correct_mode == COR_MANUAL)
								{
									
									correct_mode = COR_AUTO;
									time_to_sleep = TIME_TO_SLEEP;
									rgb_set(0,50,0,2,1000);
									correct_value(cor_feedback);
									stop_timer();
									start_timer(time_feedback);
									fds_update_value(&correct_mode, file_id, fds_rk_corr_mode);
								}
								else if (correct_mode == COR_AUTO)
									{
										adc_value = 0;
										rgb_set(50,0,0,2,1000);
										time_to_sleep = 0;
										correct_mode = COR_MANUAL;
										correct_value(cor_feedback);
										stop_timer();
										start_timer(time_feedback);
										fds_update_value(&correct_mode, file_id, fds_rk_corr_mode);
									}
							}
							
						  else if(counter_for_set_mode == 5)
								
								{
									set_up_mode_indicate();			
									counter_for_set_mode = 0;
								}
						}
						
						if(pin_in1_is_release || pin_in2_is_release || pin_in3_is_release)
						{
							counter_for_set_mode = 0;
						}
					
					
				}						
				
}

