/*****************************************************************************
* String Test Control Program for D'Addario String Test Device 
* 
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


stringPCControls TestControl;
double stringFrequencies[6];
double stringTensions[6];
int stringBreak[6] = {0};

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
	
	int inputString = atoi(argv[1]);
	readTestCon();	
	TestControl.initializePort();
	sleep(2);
	TestControl.sampleString(inputString);
	cout<<"Frequency:"<<TestControl.getFreq(stringFrequencies[inputString])<<endl;
	TestControl.closePort();
	
	
	
	
	return 0;
}

