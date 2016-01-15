#include <stdio.h>
#include <syslog.h>
#include <mraa.h>
#include <signal.h>
#include <unistd.h>
#include <mraa/gpio.h>
#include <time.h>
#include <mraa/aio.h>

/* all code sohuld be non blocking*/

typedef unsigned char U_CHAR;
typedef unsigned int U_INT;

U_INT check_sec_pass(U_INT second, time_t time_old )
{
	U_INT check_res = 0;
	time_t time_now = time();
	double time_d_diff = difftime( time_now, time_old);
	U_INT time_diff = (U_INT) time_d_diff;

	if (time_diff > second)
	{
		check_res = 1;
	}
	else
	{
		check_res = 0;
	}


	return check_res;
}

U_INT get_temp(void)
{
	U_INT temp = 199;

	// get temperature from sensor read from /in_voltageX_raw
	return temp;
}

void send_temp(U_INT temp)
{
	//send temp date to server
	return 0;
}

void climate_ctrl(U_INT temp)
{
	//climate control base on temperature
	return 0;
}

U_INT check_alram_sw(void)
{
	// return state of the alram switch.
	U_INT sw_stat = 0;
	// sw_stat = // state of switch (on/off)

	return sw_stat;
}

U_INT motion_stat(void) //PIR
{
	U_INT pir_out = 0;

	pir_out = ;//pin out of pir gpio with transistor boast

	return pir_out;
}

void sound_buzz(U_INT stat)
{
	if (stat == 1)
	{
		//turn on alram
	}
	else if (stat == 0)
	{
		//turn off alram
	}
	return 0;
}

U_INT prox_stat(void)
{
	U_INT prox_val = 0;
	U_INT tocount = 0;
	struct timespec timtrig, timecho, timloop ,timwait, timdump;

	timtrig.tv_sec = 0;
	timtrig.tv_nsec = 12000;

	timecho.tv_sec = 0;
	timecho.tv_nsec = 600000L;

	timloop.tv_sec = 0;
	timloop.tv_nsec = 1000;	//1 us per loop

	timwait.tv_sec = 0;
	timwait.tv_nsec = 50000000L; //50ms wait

	//high to trig pin
	nanosleep(&timtrig, &timdump);
	//low to trg pin

	while( 0 == /*echo pin*/ && tocount < 10000) //waiting echo to go high, time out 10000 us/ 10ms
	{
		nanosleep(&timloop, &timdump);
		tocount++;
	}
	
	nanosleep(&timecho, &timdump); 

	//if echo pin = high, prox val = 0, vice versa

	if(tocount > 9500) //timeout control
	{
		prox_val = 0;
	}

	nanosleep(&timwait, &timdump); 
	return prox_val;
}

void door_motor(U_INT dir_opt)
{
	if ( 1 == dir_opt)
	{
		//open door
	}
	if ( 0 == dir_opt)
	{
		//close door
	}

	return 0;
}

int main (void)
{
	U_INT sense_temp = 999; //initialize to an impossible but non zero value.
	U_INT temp_5sgone = 1;
	time_t temp_time = 0;

	U_INT alram_sel = 0;
	U_INT motion_det= 0;
	U_INT alram_1sgone = 1;
	time_t alram_time = 0;

	U_INT door_pos = 0;
	U_INT door_3sgone = 1;
	time_t door_time = 0;


	while (1)
	{
		if ( 1 == temp_5sgone )
		{
			sense_temp = get_temp();
			send_temp(sense_temp);
			climate_ctrl(sense_temp);
			
			temp_time= time();
			temp_5sgone = 0;
		}

		temp_5sgone = check_sec_pass( 5, temp_time);

		
		alram_sel = check_alram_sw();
		motion_det = motion_stat();

		if (1 == alram_sel )
		{
			if ( 1 == motion_det )
			{
				sound_buzz(1);
			}
		}
		else if ( 0 == alram_sel )
		{
			if ( 1 == alram_1sgone )
			{
				sound_buzz(0);	
			}	
			
			if ( 1 == motion_det)
			{
				sound_buzz(1);

				alram_time = time();
				alram_1sgone = 0;

			}
		}
		alram_1sgone = check_sec_pass( 1 , alram_time);

		if ( (0 == alram_sel) && (1 == prox_stat) && (0 == door_pos) )
		{
			door_motor(1);
			door_pos = 1;

			door_time = time();
			door_3sgone = 0;
		}

		else if ((0 == alram_sel) && (0 == prox_stat) && (1 == door_pos) )
		{
			
			door_3sgone = check_sec_pass( 3, door_time);
			
			if ( 1 == door_3sgone )
			{
				door_motor(0);
				door_pos = 0;
			}
		}
	}
	return 0;
}

