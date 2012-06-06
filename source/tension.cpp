/*****************************************************************************
* Tighten Program for D'Addario String Test Device 
* This program takes 1 argument string and returns tension
* ./tension tring steps
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
	TestControl.initializePort();
	cout<<TestControl.sampleTens(inputString)<<endl;
	TestControl.closePort();
	
	return 0;
}

