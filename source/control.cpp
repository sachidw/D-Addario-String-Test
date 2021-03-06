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


double stringFrequencies[6];
double stringTensions[6];
int stringBreak[6] = {0};
stringPCControls TestControl;
int timeBetweenTest=0;
int numberofTest=0;
double scaleLength=0;


double changeRate[10][2];
int front=0;
int back=0;


void tuneUp(int testStrng)
{


}




void sBreak(int strng)
{
	stringBreak[strng] =1;
	cout<<"String Break on: "<<strng<<endl;
	

}


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

int main ()
{
	double freq;
	string readString;
	ofstream rawDat;
  	rawDat.open ("tuneSteps.dat"); 
	readTestCon();
	TestControl.initializePort();
	sleep(2);
	for(int i =0; i<10; i++)
	{	
	TestControl.motorStep(0, 9000, 0);	
	TestControl.motorStep(1, 9000, 0);
	TestControl.motorStep(2, 9000, 0);
	TestControl.resetPosition(0);
	TestControl.resetPosition(1);
	TestControl.resetPosition(2);	
	}	
	//TestControl.resetPosition(0);
	//tuneUp(0);
	//tuneUp(1);
	//tuneUp(2);
	//tuneUpv2(1);
	rawDat<<TestControl.getElapsedSteps(0)<<endl;
	rawDat<<TestControl.getElapsedSteps(1)<<endl;
	rawDat<<TestControl.getElapsedSteps(2)<<endl;

	rawDat.close();
	TestControl.closePort();
	
	
	
	
	return 0;
}


