/*****************************************************************************
* String Test Control Program for D'Addario String Test Device 
* Bring string or strings to tension in testconfig file
* 
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

double stringFrequencies[6];
double stringTensions[6];
int stringBreak[6] = {0};
stringPCControls TestControl;
int timeBetweenTest=0;
int numberofTest=0;
double scaleLength=0;

void readTestCon()
{
	string readstring;
	ifstream config;
  	config.open ("testconfig.txt");
	for(int i=0; i<6; i++)
	{	
		getline(config, readstring);
		stringFrequencies[i]=atof(readstring.c_str());
	}
	for(int i=0; i<6; i++)
	{	
		getline(config, readstring);
		stringTensions[i]=atof(readstring.c_str());
	}
	getline(config, readstring);
	timeBetweenTest =atoi(readstring.c_str());
	numberofTest =atoi(readstring.c_str());
		
	for(int i=0; i<6; i++)
	{	
		
		cout<<"String "<<i<<"\tFrequency: "<<stringFrequencies[i]<<"   "<<"\tTension: "<<stringTensions[i]<<endl;
	}/*
	cout<<"Time between tests: "<<timeBetweenTest<<"\tNumber of tests: "<<numberofTest<<"\tTotal time: ";
	cout<<((timeBetweenTest*numberofTest)/60)<<" hours"<<endl;
	*/
	config.close();	
}

int main (int argc,char *argv[])
{
	

	double tension;
	readTestCon();

	TestControl.initializePort();
	if(argc == 1)
	{
		

	}
	else
	{
		int inputString = atoi(argv[1]);
				
		tension=TestControl.sampleTens(inputString);
		cout<<"Desired Tension: "<<stringTensions[inputString]<<endl;
		while(tension < (stringTensions[inputString]-2)  && tension < 98 )
		{	
			tension=TestControl.sampleTens(inputString);		
			cout<<tension<<endl;
			if(tension < stringTensions[inputString])
				TestControl.motorStep(inputString, 250, 0);
			tension=TestControl.sampleTens(inputString);		
			cout<<tension<<endl;
		}


	}

	TestControl.closePort();
	
	
	
	
	return 0;
}


