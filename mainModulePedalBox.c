/********************************************************           
    * Main Module Program - Pedal Box: 					   *   
    *                                                      *   
    * Author:  Kai Strubel                                 *   
    *                                                      *   
    * Purpose:  Takes inputs from Pedal box 		       *   
    *           Sets throttle level			               *   
	*			handles safety checks 					   *
    ********************************************************/  
	
int mainModulePedalBoxMessageHandler(int brake_level, int throttle_level) {
	// Inputs:
	//		brake_level from pedalbox potentiometer
	//		throttle_level from pedalbox potentiometer
	// 		current time
	// 		EOR flag
	//		APPS_Implausible flag
	// Global Variables:
	// 		torque
	//		brake_flage
	// 		MMPB_TIME time this function was last run
	// Constants:
	//		timeout - time for CAN tx to timeout
	// set function time stamp
	MMPB_TIME = current_time;
	if (EOR) { 
		torque = 0;
		// send CAN message to motor, set to Low Power State
		sendMotorMessage();

		// Set Pin ? to high
		/* TODO */
		return NULL;
	} 
	if (APPS_Implausible) {
		torque = 0;
		return NULL;
	}
	if (brake_level > 5%) {
		// signal LV to turn on break light
		HAL_CAN_TRANSMIT()
		/* TODO */
		if (throttle_level > 25%) {
			brake_flag = true;
		}
	}
	if (brake_flag) {
		if(throttle > 5%){
			torque = 0;
			return NULL;
		}
		brake_flag = false;
	}

	// set throttle
	torque = throttle_level;

	return 1;

}

int sendMotorMessage () {
		// used to send CAN message to motor
	
		CAN_HandleTypeDef messageStructure;
		CanTxMsgTypeDef message;
		// Fills out messageStructure fields
		/*
			CAN_TypeDef * Instance // is register of base address
			CAN_InitTypeDef Init // CAN required parameters
		*/
		messageStructure.CanTxMsgTypeDef = &message; // pointer to transmit structure
		messageStructure.CanRxMsgTypeDef = NULL; // pointer to receive structre
		/*
			__IO HAL_CAN_StateTypeDef State // Can communication state
			HAL_LockTypeDef Lock
			__IO uint32_t ErrorCode
		*/
		
		// Fills out message structure
		/*
		message.StdId = ;
		message.ExtId = ;
		message.IDE = ;
		message.RTR = ;
		message.DLC = ;
		message.Data = ;
		*/
		HAL_CAN_TRANSMIT(&messageStructure, Timeout);

		return 1;
}