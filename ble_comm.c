#include "ble_comm.h"



uint8_t ble_string_put[10] = "";

void ble_debug(uint8_t * ble_buffer)
	{
		
						uint32_t send_num = 0;
						uint16_t debug_number = atoi((char*)(ble_buffer+1));
						
		
						
		
						switch (debug_number)
							
						{
							
							case 1:
								send_num = clock_counter;
								break;
							
							case 2:
								send_num = life_counter/60;			// passed minutes
								break;
							
							case 3:
								send_num = activate_status;	
							break;
							
							case 31:
								send_num = cur_cor_max;	
							break;
							
							case 32:
								send_num = cur_hr_max;	
							break;
							
							case 33:
								send_num = cur_res_max;	
							break;
							
							case 4:
								send_num = corr_counter;	
								break;
							
							case 5:
								send_num = power_down_count;	
								break;
							
							case 6:
								send_num = cal_zero_value;	
								break;
							
							case 7:
								send_num = cal_turn_on;	
								break;
							
							case 8:
								send_num = fds_clear_counter;	
								break;
							
							case 9:
								//ble_string_put = device_name;
								memcpy(ble_string_put, device_name, sizeof(device_name));
								break;
							
							case 10:
								
								break;
							
							
						}
						
						if(send_num > 0)
						{
							sprintf((char*)ble_string_put, "%d", send_num);
						}
		}
		
		





