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

/* Function Declarations -----------------------------------------------------*/

int main(void)
{
	// Store Thermo Data
	// Needs to determine timestamp
	// if data is from thermo
	// {
	//		FILE* fp;
	//		fp = fopen('thermodata.txt", "w");
	//		int data = // recieved data
	//		int len = 2 + data / 10
	//		char* str = malloc(sizeof(*str) * len);
	//		sprintf(str, "%d\n", data);
	//		fwrite(str, 1, sizeof(str), fp);
	//		fclose(fp);
	//	}
	
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