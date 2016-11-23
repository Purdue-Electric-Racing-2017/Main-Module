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
	// Store Wheel Speed, send timestamp
	
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
			THROTTLE = 0.5;
			return EXIT_SUCCESS;
		}
		
		// Reset Constants
		SLIPFLAG = 0;
		THROTTLE = 1;
	}
	return EXIT_SUCCESS;
}