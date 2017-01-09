/***************************************************************************
*
*     File Information
*
*     Name of File: wheel_control.c
*
*     Authors (Include Email):
*       1. Raymond Dong			dong155@purdue.edu
*
*     File dependents: (header files, flow charts, referenced documentation)
*       1. wheel_control.h
*
*     File Description: 
*		Records Wheel Thermo and Speed data. 
*		Handles launch control.
*
***************************************************************************/

#include "wheel_control.h"

/***************************************************************************
*
*     Function Information
*
*     Name of Function: strdup
*
*     Programmer's Name: Raymond Dong
*
*     Function Return Type: char*
*
*     Parameters (list data type, name, and comment one per line):
*		1. char* msg					// Message to Duplicate
*
*     Global Dependents:
*
*     Function Description:
*		Duplicate String
*
***************************************************************************/

char* strdup(char* msg)
{
	return strcpy(malloc(sizeof(*msg) * (strlen(msg) + 1)), msg);
}

/***************************************************************************
*
*     Function Information
*
*     Name of Function: thermo
*
*     Programmer's Name: Raymond Dong
*
*     Function Return Type: void
*
*     Parameters (list data type, name, and comment one per line):
*		1. char** error					// Error Handling
*		2. int wheel_num				// Wheel Number
*		3. double* thermo_array			// Array of thermo data
*
*     Global Dependents:
*
*     Function Description:
*		Stores Thermo data
*
***************************************************************************/

void thermo(char** error, int wheel_num, double* thermo_array)
{
	// Check if Error Exists Beforehand
	if(*error != NULL)
	{
		return;
	}
	
	// Gather Current Time
	time_t currrent;
	if(current == (time_t - 1))
	{
		if(*error == NULL)
		{
			char* msg = "Failure to Obtain Current Time.";
			*error = strdup(msg);
		}
		return;
	}
	
	// Determine which File to Write
	switch(wheel_num)
	{
		case 1:
			char* file_name = "Thermo_Wheel_1.txt";
			break;
		case 2
			char* file_name = "Thermo_Wheel_2.txt";
			break;
		case 3
			char* file_name = "Thermo_Wheel_3.txt";
			break;
		case 4
			char* file_name = "Thermo_Wheel_4.txt";
			break;
		default:
			if(*error == NULL)
			{
				char* msg = "Invalid Wheel Number.";
				*error = strdup(msg);
			}
			return;
	}
	
	// Open File
	FILE* fp;
	fp = fopen(file_name, "w");
	if(fp == NULL)
	{
		if(*error == NULL)
		{
			char* msg = "Failure to Open File.";
			*error = strdup(msg);
		}
		return;
	}
	
	// Gather Timestamp and Write
	char* time_s = ctime(&current);
	if(time_s == NULL)
	{
		if(*error == NULL)
		{
			char* msg = "Failure to Obtain Timestamp.";
			*error = strdup(msg);
		}
		return;
	}
	fwrite(time_s, 1, sizeof(*time_s), fp);
	
	// Convert Data to String
	char dest[THERMO_NUM_CHAR];
	double data[THERMO_NUM_DATA] = thermo_array;
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
	return;
}

/***************************************************************************
*
*     Function Information
*
*     Name of Function: speed
*
*     Programmer's Name: Raymond Dong
*
*     Function Return Type: void
*
*     Parameters (list data type, name, and comment one per line):
*		1. char** error					// Error Handling
*		2. int wheel_num				// Wheel Number
*		3. double wheel_speed			// Wheel Speed (rad / sec)
*		4. double* wheel_pointer		// Wheel Pointer
*
*     Global Dependents:
*
*     Function Description:
*		Stores Speed data
*
***************************************************************************/

void speed(char** error, int wheel_num, double wheel_speed, double* wheel_pointer)
{
	// Check if Error Exists Beforehand
	if(*error != NULL)
	{
		return;
	}
	
	// Gather Current Time
	time_t currrent;
	if(current == (time_t - 1))
	{
		if(*error == NULL)
		{
			char* msg = "Failure to Obtain Current Time.";
			*error = strdup(msg);
		}
		return;
	}
	
	// Determine which File to Write
	switch(wheel_num)
	{
		case 1:
			char* file_name = "Speed_Wheel_1.txt";
			break;
		case 2
			char* file_name = "Speed_Wheel_2.txt";
			break;
		case 3
			char* file_name = "Speed_Wheel_3.txt";
			break;
		case 4
			char* file_name = "Speed_Wheel_4.txt";
			break;
		default:
			if(*error == NULL)
			{
				char* msg = "Invalid Wheel Number.";
				*error = strdup(msg);
			}
			return;
	}
	
	// Open File
	FILE* fp;
	fp = fopen(file_name, "w");
	if(fp == NULL)
	{
		if(*error == NULL)
		{
			char* msg = "Failure to Open File.";
			*error = strdup(msg);
		}
		return;
	}
	
	// Gather Timestamp and Write
	char* time_s = ctime(&current);
	if(time_s == NULL)
	{
		if(*error == NULL)
		{
			char* msg = "Failure to Obtain Timestamp.";
			*error = strdup(msg);
		}
		return;
	}
	fwrite(time_s, 1, sizeof(*time_s), fp);
	
	// Convert Data to String
	char src[SPEED_MAX_CHAR];
	sprintf("%f\n", wheel_speed);
	
	// Record Data
	wheel_pointer = wheel_speed;
	fwrite(dest, 1, SPEED_MAX_CHAR, fp);
	fclose(fp);
	return;
}

/***************************************************************************
*
*     Function Information
*
*     Name of Function: traction
*
*     Programmer's Name: Raymond Dong
*
*     Function Return Type: void
*
*     Parameters (list data type, name, and comment one per line):
*		1. char** error					// Error Handling
*		2. double* pointer1				// Wheel 1 Pointer
*		3. double* pointer2				// Wheel 2 Pointer
*		4. double* pointer3				// Wheel 3 Pointer
*		5. double* pointer4				// Wheel 4 Pointer
*		6. double* throttle				// Throttle Coefficent
*
*     Global Dependents:
*
*     Function Description:
*		Traction Control
*
***************************************************************************/

void traction(char** error, double* pointer1, double* pointer2, double* pointer3, double* pointer4, double* throttle)
{
	// Average Front and Back
	int front = (pointer1 + pointer2) / 2;
	int back = (pointer3 + pointer4) / 2;
	
	// Check for Slip
	if(back > (front * MAX_PERCENT_SLIP))
	{
		throttle = (back - front) / front;
	}
}