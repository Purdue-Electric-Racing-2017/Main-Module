/***************************************************************************
*
*     File Information
*
*     Name of File: mainModulePedalBox.c
*
*     Authors (Include Email):
*       1. Kai Strubel       kstrubel@purdue.edu
*
*     File dependents: (header files, flow charts, referenced documentation)
*       1.
*
*     File Description: Takes inputs from the pedal box,
*       handles safety checks, and sets throttle level
*
***************************************************************************/
#define MAX_BRAKE_LEVEL 42.42424242 // BOGUS VALUE
#define MAX_THROTTLE_LEVEL 42.42424242 // BOGUS VALUE
#define LC_THRESHOLD 10 // BOGUS VALUE
xQueueHandle Global_Pedal_Box_Queue_Handle = 0; 

typedef struct _pedalbox_msg {
   bool EOR; // rule 2.4.6: Encoder out of range
   bool APPS_Implausible; // rule 2.3.5
   float throttle_level;
   float brake_level;
   
} Pedalbox_msg_t;
	
int pedalBoxMsgHandler() {
/***************************************************************************
*
*     Function Information
*
*     Name of Function: pedalBoxMessageHandler
*
*     Programmer's Name: Kai Strubel
*
*     Function Return Type: int
*
*     Parameters (list data type, name, and comment one per line):
*       1.Pedalbox_msg_t msg 
			brake_level from pedalbox potentiometer
*			throttle_level from pedalbox potentiometer
*			APPS_Implausible flag
*			EOR flag
*		2.
*       
*      Global Dependents:
*	    1.torque
*		2.brake_flag
*		3.MMPB_TIME time this function was last run 
*		4.current_time
*		5.Global_Pedal_Box_Queue_Handle
*		6.g_throttle_level
*
*     Function Description:
*			Takes input from pedal box, runs safetly check, sets torque
***************************************************************************/
	while (1) {
		Pedalbox_msg_t msg;

		if(xQueueReceive(Global_Pedal_Box_Queue_Handle, &msg, 1000)){

			// set function time stamp
			MMPB_TIME = current_time;

			if (msg.EOR) { // rule 2.4.6
				torque = 0;
				// send CAN message to motor, set to Low Power State
				// sendMotorMessage();

				// toggle frg/run fre
				HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_11);
				HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_12);
				
				// pin 29 port e11
				// pin 30 port e12
				
				
			} 
			
			if (msg.APPS_Implausible) {
				torque = 0;
				// send CAN message to motor, set to Low Power State
				// sendMotorMessage();

				// toggle frg/run fre
				HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_11);
				HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_12);
				// pin 29 port e11
				// pin 30 port e12
				/* TODO */
			
			}
			if (msg.brake_level > 0.05*MAX_BRAKE_LEVEL) {
				// signal LV to turn on break light
				// Brake light 
				
			
				HAL_GPIO_WritePin(GPIOP, GPIO_PIN_E7, GPIO_PIN_SET);
				// set pin PE7

				/* TODO */

				if (msg.throttle_level > 0.25 * MAX_THROTTLE_LEVEL) {
					brake_flag = true;
				}
			}

			else {
				// turn Brake light off
				HAL_GPIO_WritePin(GPIOP, GPIO_PIN_E7, GPIO_PIN_RESET);
			}

			if (brake_flag) {
				if(msg.throttle_level > 0.05 * MAX_THROTTLE_LEVEL){
					torque = 0;
				}
				brake_flag = false;
			}

			// set throttle
			g_throttle_level = msg.throttle_level;
			torque = msg.throttle_level;

				
		}
	}
		
	

}

int mainModuleTorque() {
/***************************************************************************
*
*     Function Information
*
*     Name of Function: mainModuleTorque
*
*     Programmer's Name: Kai Strubel
*
*     Function Return Type: int
*
*     Parameters (list data type, name, and comment one per line):
*       1.
*       
*      Global Dependents:
*	    1.bool launchControl
*		2.float apps_max
*		3.float torque
*		4.float scaleFactor
*
*     Function Description:
*		Calculates the torque based of slipping factor and throttle position
*		
***************************************************************************/
	while(1) {

		if (launchControl) {
			if(torque > apps_max) {
				// apps - accelerator pedal position system
				// bse - breake system encoder
				apps_max = torque;
				torque *= scaleFactor;	
			}
			else {
				if (apps_max - torque > LC_THRESHOLD) {
					launchControl = false;
				}
				else {
					torque *= scaleFactor;
					
				}
			}
			
		}

		

		sendMotorMessage();

		vTaskDelay(1000);
	}
}

int mainModuleTimeCheckIdle() {
/***************************************************************************
*
*     Function Information
*
*     Name of Function: mainModuleTimeCheckIdle
*
*     Programmer's Name: Kai Strubel
*
*     Function Return Type: int
*
*     Parameters (list data type, name, and comment one per line):
*       1.
*       
*      Global Dependents:
*	    1.bool launchControl
*		2.float MMPB_TIME time pedal box message handler function was last run 
*		3.float MMWM_TIME time wheel module handler function was last run
*		4.float torque
*		5.float currentTime
*
*     Function Description:
*		Checks if wheel module and pedal box are still communicating
*		
***************************************************************************/
	while (1) {
		if (currentTime - MMPB_TIME > LC_THRESHOLD) {
			torque = 0;
			// low power state?
			// error
		}
		if (currentTime - MMWM_TIME > LC_THRESHOLD) {
			launchControl = 0;
			//error
		}
		vTaskDelay(500);
	}
}

int heartbeatIdle() {
/***************************************************************************
*
*     Function Information
*
*     Name of Function: heartbeatIdle
*
*     Programmer's Name: Kai Strubel
*
*     Function Return Type: int
*
*     Parameters (list data type, name, and comment one per line):
*       1.
*       
*      Global Dependents:
*
*     Function Description:
*		Heart beat to communicate that main module is alive
*		
***************************************************************************/
	// write to GPIO
	while (1) {
		HAL_GPIO_WritePin(GPIOP, GPIO_PIN_E7, GPIO_PIN_SET);
		vTaskDelay(500);
	}
	
}

int main(void) {

	Global_Pedal_Box_Queue_Handle = xQueueCreate(3, sizeof(Pedalbox_msg_t));

	/* Create Tasks */
	xTaskCreate(pedalBoxMsgHandler, (signed char*) "pedalBoxMsgHandler", 1024, NULL, 1, NULL);
	xTaskCreate(mainModuleTimeCheckIdle, (signed char*) "mainModuleTimeCheckIdle", 1024, NULL, 1, NULL);
	xTaskCreate(mainModuleTorque, (signed char*) "mainModuleTorque", 1024, NULL, 1, NULL);
	xTaskCreate(heartbeatIdle, (signed char*) "heartbeatIdle", 1024, NULL, 1, NULL);

	vTaskStartScheduler();

	return 0;

 }
