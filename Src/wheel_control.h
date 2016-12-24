/***************************************************************************
*
*     File Information
*
*     Name of File: wheel_control.h
*
*     Authors (Include Email):
*       1. Raymond Dong			dong155@purdue.edu
*
*     File dependents: (header files, flow charts, referenced documentation)
*
*     File Description: 
*		Header file for wheel_control.c
*
***************************************************************************/

// Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Constants
#define THERMO_NUM_ROW 				4	// Number of Rows from Thermo Data
#define THERMO_NUM_COL 				16	// Number of Columns from Thermo Data
#define THERMO_NUM_DATA 			64	// Number of Data from Thermo Data
#define THERMO_NUM_CHAR 			512	// Number of Char to Record
#define THERMO_MAX_CHAR_PER_ELEM	8	// Max Number of Char per Element to Record
#define THERMO_MAX_CHAR_PER_BYTE	3	// Max Number of Char per Byte to Record
#define SPEED_MAX_CHAR				12	// Max Number of Char

// Function Declarations
char* strdup(char* msg);
void thermo(char** error, int wheel_num, double* thermo_array);
void speed(char** error, int wheel_num, double wheel_speed);
void launch(char** error);