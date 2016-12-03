/**
  ******************************************************************************
  * File Name          : wheel_control.c
  * Description        : Process Data from Wheel Module
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f7xx_hal.h"
#include "can.h"
#include "dac.h"
#include "gpio.h"
// Essential Libraries: stdio, stdlib, string, time

/* Constants -----------------------------------------------------------------*/
#THERMO_NUM_ROW 				4	// Number of Rows from Thermo Data
#THERMO_NUM_COL 				16	// Number of Columns from Thermo Data
#THERMO_NUM_DATA 				64	// Number of Data from Thermo Data
#THERMO_NUM_CHAR 				512	// Number of Char to Record
#THERMO_MAX_CHAR_PER_ELEM	 	8	// Max Number of Char per Element to Record
#THERMO_MAX_CHAR_PER_BYTE 		3	// Max Number of Char per Byte to Record

/* Function Declarations -----------------------------------------------------*/

int main(void)
{
	// Store Thermo Data
	if ()
	{
		time_t currrent;
		if(current == (time_t - 1))
		{
/* Error Handle --------------------------------------------------------------*/
			return EXIT_FAILURE;
		}
		
		FILE* fp;
		fp = fopen('thermodata.txt", "w");
		if(fp == NULL)
		{
/* Error Handle --------------------------------------------------------------*/
			return EXIT_FAILURE;
		}
		
		char* time_s = ctime(&current);
		if(time_s == NULL)
		{
/* Error Handle --------------------------------------------------------------*/
			return EXIT_FAILURE;
		}
		fwrite(time_s, 1, sizeof(*time_s), fp);
		
		char dest[THERMO_NUM_CHAR];
/* Incomplete ----------------------------------------------------------------*/
		int data[THERMO_NUM_DATA] = ;
/* ---------------------------------------------------------------------------*/
		
		for(int i = 0; i < THERMO_NUM_ROW; i++)
		{
			for(int j = 0; j < THERMO_NUM_COL; j++)
			{
				char src[THERMO_MAX_CHAR_PER_ELEM];
				char left_string[THERMO_MAX_CHAR_PER_BYTE];
				char right_string[THERMO_MAX_CHAR_PER_BYTE];
				
				// Divide Left and Right Side of Decimal
				int left = data[j + i * THERMO_NUM_COL] / 1000;
				int right = data[j + i * THERMO_NUM_COL] % 1000;
				
				// Convert to String
				if(right < 10)
				{
					sprintf(right_string, "%d  ", right);
				}
				else if(right < 100)
				{
					sprintf(right_string, "%d ", right);
				}
				else
				{
					sprintf(right_string, "%d", right);
				}
				
				// Convert to String
				if(left < 10)
				{
					sprintf(left_string, "%d  ", left);
				}
				else if(right < 100)
				{
					sprintf(left_string, "%d ", left);
				}
				else
				{
					sprintf(left_string, "%d", left);
				}
				
				// Combine Strings
				if(j < THERMO_NUM_COL - 1)
				{
					sprintf(src, "%s.%s\t", left_string, right_string);
				}
				else
				{
					sprintf(src, "%s.%s\n", left_string, right_string);
				}
				strcat(dest, src);
			}
		}
		
		// Record Data
		fwrite(dest, 1, THERMO_NUM_CHAR, fp);
		fclose(fp);
	}
	
	// Store Wheel Speed, send timestamp
	// Needs to determine wheel and timestamp
	// if data is from wheel
	// {
	//		FILE* fp;
	//		fp = fopen('wheeldata.txt", "w");
	//		int data = // recieved data
	//		int len = 2 + data / 10
	//		char* str = malloc(sizeof(*str) * len);
	//		sprintf(str, "%d\n", data);
	//		fwrite(str, 1, sizeof(str), fp);
	//		fclose(fp);
	//	}
	
	if(LAUNCHMODE == 1)
	{
		// Front Wheel Torque
		int torq1;
		int torq2;
		
		// Back Wheel Torque
		int torq3;
		int torq4;
		
		// Average Front and Back
		int torqF = (torq1 + torq2) / 2;
		int torqB = (torq3 + torq4) / 2;
		
		// Check for Slip
		if(torqB > torqF * 1.05)
		{
			SLIPFLAG = 1;
			THROTTLE = (torqB - torqF) / torqF;
			// Testing required for value
			return EXIT_SUCCESS;
		}
		
		// Reset Constants
		SLIPFLAG = 0;
		THROTTLE = 1;
	}
	return EXIT_SUCCESS;
}