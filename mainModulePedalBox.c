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
xQueueHandle Global_Queue_Handle = 0; 

typedef struct _pedalbox_msg {
   bool EOR;
   bool APPS_Implausible;
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
*		5.Gloabal_Queue_Handle
*
*     Function Description:
*			Takes input from pedal box, runs safetly check, sets torque
***************************************************************************/
	while (1) {
		Pedalbox_msg_t msg;

		if(xQueueReceive(Gloabal_Queue_Handle, &msg, 1000)){
			// set function time stamp
			MMPB_TIME = current_time;

			if (msg.EOR) { 
				torque = 0;
				// send CAN message to motor, set to Low Power State
				// sendMotorMessage();

				// Set Pin ? to high
				/* TODO */
				continue;
			} 
			if (msg.APPS_Implausible) {
				torque = 0;
				continue;
			}
			if (msg.brake_level > 0.05*MAX_BRAKE_LEVEL) {
				// signal LV to turn on break light
				HAL_CAN_TRANSMIT()
				/* TODO */

				if (msg.throttle_level > 0.25 * MAX_THROTTLE_LEVEL) {
					brake_flag = true;
				}
			}

			if (brake_flag) {
				if(msg.throttle_level > 0.05 * MAX_THROTTLE_LEVEL){
					torque = 0;
					continue;
				}
				brake_flag = false;
			}

			// set throttle
			torque = throttle_level;

			continue;	
		}
	}
		
	

}

int mainModuleIdle() {
/***************************************************************************
*
*     Function Information
*
*     Name of Function: mainModuleIdle
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
*		3.float MMPB_TIME time pedal box message handler function was last run 
*		4.float MMWM_TIME time wheel module handler function was last run
*		5.float torque
*		6.float scale factor
*		7.float currentTime
*
*     Function Description:
*		Idle function for main module, checks that other functions are 
*		being run, 
***************************************************************************/
	while(1) {

		if (launchControl) {
			if(torque > apps_max) {
				// apps - accelerator pedal position system
				// bse - breake system encoder
				apps_max = torque;
				torque *= scaleFactor;
				sendMotorMessage;
				continue;
			}
			else {
				if (apps_max - torque > LC_THRESHOLD) {
					launchControl = false;
				}
				else {
					torque *= scaleFactor;
					sendMotorMessage;
					continue;
				}
			}
			
		}

		if (currentTime - MMPB_TIME > LC_THRESHOLD) {
			torque = 0;
			// low power state?
			// error
		}

		if (currentTime - MMWM_TIME > LC_THRESHOLD) {
			launchControl = 0;
			//error
		}

		sendMotorMessage();

		vTaskDelay(1000);
	}
}

int main(void) {
	Gloabal_Queue_Handle = xQueueCreate(3, sizeof(Pedalbox_msg_t));

	/* Create Tasks */
	xTaskCreate(pedalBoxMsgHandler, (signed char*) "pedalBoxMsgHandler", 1024, NULL, 1, NULL);
	xTaskCreate(mainModuleIdle, (signed char*) "mainModuleIdle", 1024, NULL, 1, NULL);

	vTaskStartScheduler();

	return 0;

 }
