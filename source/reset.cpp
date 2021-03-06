/*****************************************************************************
* Tighten Program for D'Addario String Test Device 
* This program takes 1 or no arguments and rezeros all or just selected string
* ./reset
* ./reset string
* 
*
* Created by Matt Fors
* Binghamton University
* mfors1@binghamton.edu
******************************************************************************/

#include "stringPCControls.h"
#include <iostream>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>


stringPCControls TestControl;
int main (int argc,char *argv[])
{
	TestControl.initializePort();
	if(argc == 1)
	{
		TestControl.resetPosition(0);
		TestControl.resetPosition(1);
		TestControl.resetPosition(2);	

	}
	else
	{
		int inputString = atoi(argv[1]);
		TestControl.resetPosition(inputString);	
	}

	TestControl.closePort();
	
	
	
	
	return 0;
}


