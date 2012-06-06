/*****************************************************************************
* Tighten Program for D'Addario String Test Device 
* This program takes two arguments string and steps
* ./tighten string steps
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
	int inputString = atoi(argv[1]);
	int inputSteps = atoi(argv[2]);
	TestControl.initializePort();
	//sleep(2);
	cout<<"STRING: "<<inputString<<endl;
	cout<<"STEPS:  "<< argv[2] <<endl;
	TestControl.motorStep(inputString, inputSteps, 1);
	
	TestControl.closePort();
	
	
	
	
	return 0;
}


