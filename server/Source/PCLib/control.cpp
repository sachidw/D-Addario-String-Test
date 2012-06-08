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


void sBreak(int strng)
{
	stringBreak[strng] =1;
	cout<<"String Break on: "<<strng<<endl;
	

}

/****************************************************************** 
 *	v2 of string tune up function	
 *	This function tunes specified string to desired frequency
 *	using a rate of change calculation. 
 *	
 ******************************************************************/  
void tuneUpv2(int testStrng)
{
	
	double prevFreq;
	double tension;
	double freq;
	int testStrng =0;
	int strng =0;
	int totalS=0;
	int stepSize=0;
	char sSteps[4]; //Step size as a string	
	double lastFreq=0;	
	double slope=0;
	int lastStepSize=0;
	/****************************************************************** 
 	*	Determine an initial rate of change by getting the change in 
 	*	pitch between 100 steps
 	******************************************************************/  
		TestControl.sampleString(testStrng);
		lastFreq=TestControl.getFreq(stringFrequencies[strng]);
		TestControl.motorStep(testStrng, "100", "0");
		TestControl.sampleString(testStrng);
		freq=TestControl.getFreq(stringFrequencies[strng]);
		cout<<"1st freq: "<<lastFreq<<"  2nd freq: "<<freq<<endl;	
		slope = (freq-lastFreq)/100.0;
		cout<<"INITIAL SLOPE: "<<slope<<endl;
	
	/****************************************************************** 
 	*	Approximate the number of steps need to read 2 Hz less than 
	*	the desired frequency base on rate of change.
 	*	
 	******************************************************************/  
	while( freq < stringFrequencies[strng]-2)
	{
		
		TestControl.sampleString(testStrng);
		freq=TestControl.getFreq(stringFrequencies[strng]);
		cout<<"FREQ: "<<freq<<endl;		
		if(freq <(lastFreq+80) && freq>(lastFreq-100)  )
		{			
				if(lastStepSize==0) //First time in loop use initial slope
				{	
					stepSize = ((stringFrequencies[0]-freq) / slope);
					cout<<"1ststepSize: "<<stepSize<<endl;	
					lastStepSize = stepSize;
				}		
				else //Calculate a new slope based on the frequency before stepping
				{		
					slope = (freq-lastFreq)/lastStepSize;
					stepSize = ((stringFrequencies[0]-freq) / slope);
					lastStepSize = stepSize;
					cout<<"stepSize: "<<stepSize<<endl;
				}
		
		
		
		
				cout<<stepSize<<"\t\t"<<freq<<"\t\t"<<slope<<endl;
								
				while(stepSize > 999)
				{
					TestControl.motorStep(testStrng, "999", "0");
					stepSize= stepSize-999;
					cout<<"stepSize: "<<stepSize<<endl;
				}
				/*Need to convert stepSize to a string*/					
					sSteps[0]= ((stepSize - (stepSize%100))/100)+48;
					sSteps[1]= (   ((stepSize%100) - ((stepSize%100)%10)) / 10       )+48;
					sSteps[2]= (((stepSize%100)%10))+48;
				
				cout<<"sSteps: "<<sSteps<<endl;
				TestControl.motorStep(testStrng, sSteps, "0");
				sleep(4); //Allow string to settle after stepping the motor
				lastFreq=freq;
			

		}
	}

		/****************************************************************** 
 		*	Reach the final frequency by stepping by 20
 		*	
 		******************************************************************/  		
		cout<<"END OF SLOPE LOOP___________________"<<endl;
		while( freq < stringFrequencies[strng])
		{
			TestControl.sampleString(testStrng);
			freq=TestControl.getFreq(stringFrequencies[strng]);
			cout<<"FREQ: "<<freq<<endl;			
			if(freq <(lastFreq+15) && freq>(lastFreq-15) && freq < stringFrequencies[strng])
			{
				TestControl.motorStep(testStrng, "020", "0");
				lastFreq=freq;
			}
		}

		
}





/****************************************************************** 
 *	This function tunes specified string to frequency outlined
 *	in the testconfig file	
 ******************************************************************/  
void tuneUp(int strng)
{
	double tension;
	double freq;
	int testStrng =strng;
/*
	string testStrng;
	if(strng==5)
		testStrng="5";
	else if(strng==4)
		testStrng="4";
	else if(strng==3)
		testStrng="3";
	else if(strng==2)
		testStrng="2";
	else if(strng==1)
		testStrng="1";
	else
		testStrng="0";*/

	/****************************************************************** 
	 *	Tighten string up 2 pounds less than tension from
	 *      test configuration. Stop at less than 100 pounds
	 *      which is max load for loadcell. Do not tension if string
	 *      is broken.	
	 ******************************************************************/  
	/*		tension=TestControl.sampleTens(testStrng);
			cout<<"Desired Tension: "<<stringTensions[strng]<<endl;
			while(tension < (stringTensions[strng]-2)  && tension < 98 && stringBreak[strng] !=1)
			{	
				tension=TestControl.sampleTens(testStrng);		
				cout<<tension<<endl;
				if(tension < stringTensions[0])
					TestControl.motorStep(testStrng, "100", "0");
				tension=TestControl.sampleTens(testStrng);		
				cout<<tension<<endl;
			}*/
	
	/****************************************************************** 
	 *	Establish a frequency of the string with certainty.
	 *      Take 5 samples and average, throw out lower than average 
	 *	frequencies and reaverage leftover frequencies.	
	 ******************************************************************/  
			cout<<"Desired freq: "<<stringFrequencies[strng]<<endl;
			TestControl.pluck(strng);
			double prevFreq;
			double establishAvg=0;
			double establishTest[5];
			double finalAvg=0;
			int establishIndex=0;
			for(int i=0; i<5; i++)	
			{	
				TestControl.sampleString(testStrng);
				freq=TestControl.getFreq(stringFrequencies[strng]);
				cout<<freq<<endl;
				establishTest[i]=freq;
				establishAvg = establishAvg+freq;
			}
			establishAvg= establishAvg/5;
	
			for(int i=0; i<5; i++)	
			{	
				if(establishTest[i]>establishAvg)
				{			
					finalAvg=finalAvg+establishTest[i];
					establishIndex++;
				}
		
			}
			finalAvg=finalAvg/establishIndex;
			cout<<"finalAvg: "<<finalAvg<<endl;
			prevFreq =finalAvg;
			freq = finalAvg;
	/****************************************************************** 
	 *	Begin tuning up the string.
	 *      
	 ******************************************************************/  
			cout<<"Increment: 100"<<endl;
			//Increment by 100 steps until withing 4 Hz of desired frequency
				while(freq < (stringFrequencies[strng]-4) && stringBreak[strng] !=1)
				{		
					TestControl.sampleString(testStrng);
					freq=TestControl.getFreq(stringFrequencies[strng]);
					cout<<"Frequency: "<<freq<<endl;
					cout<<"prevFreq: "<<prevFreq<<endl;
					//Ignore sample if it's not withing 15 Hz of previous test
					//assume data is bad
					if(freq < (stringFrequencies[strng]-3) && (freq <(prevFreq+15) && freq>(prevFreq-15))   )
					{			
						TestControl.motorStep(testStrng, "100", "0");
						prevFreq=freq;
					}

					else if(freq > (stringFrequencies[strng]-3) && (freq < (prevFreq+15) && freq> (prevFreq-15))   )
					{			
						prevFreq=freq;
					}

					else if (!(freq <prevFreq+15 && freq>prevFreq-15))
						freq=prevFreq;	
		
				}	
			//Increment by 50 steps until withing .5 Hz of desired frequency
				cout<<"Increment: 10"<<endl;
				while(freq < (stringFrequencies[strng])-.2 && stringBreak[strng] !=.2)
				{		
					TestControl.sampleString(testStrng);
					freq=TestControl.getFreq(stringFrequencies[strng] );
					cout<<"Frequency: "<<freq<<endl;
					cout<<"prevFreq: "<<prevFreq<<endl;
					//Ignore sample if it's not withing 10 Hz of previous test
					//assume data is bad
					if(freq < (stringFrequencies[strng]-1) && (freq < (prevFreq+10) && freq> (prevFreq-10))   )
					{			
						TestControl.motorStep(testStrng, "050", "0");
						prevFreq=freq;
					}
		
					else if(freq > (stringFrequencies[strng]-1) && (freq < (prevFreq+10) && freq> (prevFreq-10))   )
					{			
						prevFreq=freq;
					}

					else if (!(freq <prevFreq+10 && freq>prevFreq-10))
						freq=prevFreq;	
		
		
				}	
			//Increment by 10 steps until arrive at desired frequency
				cout<<"Increment: 5"<<endl;
				while(freq < (stringFrequencies[strng]) && stringBreak[strng] !=1)
				{		
					TestControl.sampleString(testStrng);
					freq=TestControl.getFreq(stringFrequencies[strng]);
					cout<<"Frequency: "<<freq<<endl;
					cout<<"prevFreq: "<<prevFreq<<endl;
					//Ignore sample if it's not withing .8 Hz of previous test
					//assume data is bad
					if(freq < (stringFrequencies[strng]) && (freq <(prevFreq+.8) && freq>(prevFreq-.8)))
					{			
						TestControl.motorStep(testStrng, "010", "0");
						prevFreq=freq;
					}

					else if(freq > (stringFrequencies[3]-3) && (freq < (prevFreq+.8) && freq> (prevFreq-.8))   )
					{			
						prevFreq=freq;
					}

					else if (!(freq <prevFreq+1 && freq>prevFreq-1))
						freq=prevFreq;		
		
		
				}	
	
}
/****************************************************************** 
 *	Read in test specifications from test config file
 ******************************************************************/  
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
	/*	
	for(int i=0; i<6; i++)
	{	
		
		cout<<"String "<<i<<"\tFrequency: "<<stringFrequencies[i]<<"   "<<"\tTension: "<<stringTensions[i]<<endl;
	}
	cout<<"Time between tests: "<<timeBetweenTest<<"\tNumber of tests: "<<numberofTest<<"\tTotal time: ";
	cout<<((timeBetweenTest*numberofTest)/60)<<" hours"<<endl;
	*/
	config.close();	
}

int main ()
{
	stringFrequencies[0]=246.94;
	stringTensions[0]=15;
	TestControl.initializePort();
	sleep(2);
	
	tuneUpv2(0);
	
	TestControl.closePort();
	
	
	//TestControl.resetPosition("0");
	
	return 0;
}


