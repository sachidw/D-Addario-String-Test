/*****************************************************************************
* String Test Control Program for D'Addario String Test Device 
* This program is intended to be used to periodically retun a string
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


/****************************************************************** 
*	tuneUpv2 - Attempts to tune the string up to the proper pitch
*	by estimating the rate of change of frequency per step.
******************************************************************/  
void tuneUpv2(int testStrng)
{
	double tension=0;
	double freq=0;
	double prevFreq=0;
	bool inTune= false;
	int stepSize=200;
	int prevStepSize=stepSize;
	double slope=0;	
	int aindex=0;
	double f[3]={0};
	bool checkloop=false;

	TestControl.sampleString(testStrng);
	prevFreq=TestControl.getFreq(stringFrequencies[testStrng]);
	TestControl.motorStep(testStrng, stepSize, 0);
	TestControl.sampleString(testStrng);
	freq=TestControl.getFreq(stringFrequencies[testStrng]);
	slope = (freq-prevFreq)/stepSize;
	
	cout<<"1st freq: "<<prevFreq<<"  2nd freq: "<<freq<<"  Initial Slope: "<<slope<<endl;
	
	/*Output the initial frequency to file so it can be added to data file*/
	ofstream fDat;
	fDat.open ("initialFreq.dat", ios::app);
	fDat<<freq<<endl;
	fDat.close();
	
	while(!inTune)
	{
		if(slope > .001) //If the slope is too small you run the risk of giant step sizes
			stepSize= (( (stringFrequencies[testStrng]-1)   -freq) / slope);
		if(stepSize>500) //Limit the steps size to 500 for testing purposes
			stepSize=500;

		cout<<"Step size: "<<stepSize<<endl;
		cout<<"Step MOTOR?"<<endl; //For testing purpose so we don't accidently break the string
		getchar();		   //Wait for user to approve the step size
		TestControl.motorStep(testStrng, stepSize, 0);
		sleep(4); //Let string settle
		cout<<"Done rest period"<<endl;
		prevFreq=freq;	
	
		/*Acquire a good frequency reading*/		
		do{
			TestControl.sampleString(testStrng);
			freq=TestControl.getFreq(stringFrequencies[testStrng]);
		} while (freq==0 && (freq< prevFreq-15 || freq>prevFreq+15));	
		
		slope=(freq-prevFreq)/stepSize; //Estimate the slope based on the difference in f and the last step size

		cout<<"Frequency: "<<freq<<"  PrevFrequency:"<<prevFreq<<"  Slope: "<<slope<<"  Attempt #"<<aindex<<endl;
		
		/*if(slope<0.005)
		{
			/*for(int i=0; i<3;i++)
			{			
				TestControl.sampleString(testStrng);
				double f =TestControl.getFreq(stringFrequencies[testStrng])
				cout<<"F test "<<i<<"  "<<f<<endl;
				fAvg = fAvg+f;
			}
			fAvg =fAvg/3;*/
			/*
			while(!checkloop)
			{			
				TestControl.sampleString(testStrng);
				f[0] =TestControl.getFreq(stringFrequencies[testStrng]);
				cout<<"F test 0 "<<f[0]<<endl;
				TestControl.sampleString(testStrng);
				f[1] =TestControl.getFreq(stringFrequencies[testStrng]);
				cout<<"F test 1 "<<f[1]<<endl;
				TestControl.sampleString(testStrng);
				f[2] =TestControl.getFreq(stringFrequencies[testStrng]);
				cout<<"F test 2 "<<f[2]<<endl;
				if(abs(f[0]-f[1])<.5 && abs(f[0]-f[2])<.5 && abs(f[1]-f[2]))
					checkloop=true;
			}
			checkloop=false;
			cout<<"f: "<<f<<endl;
			
		}*/		
		//freq=f;
		if(freq>=stringFrequencies[testStrng] -2 || slope < .001 )
				inTune = true;
		
		aindex++;
	}
	aindex=0;
	inTune=false;	

	/*There is difficulty with the slope method as it approached the desired frequneyc so finish with frequency range method*/
	while(!inTune)
		{
			
			if(freq < (stringFrequencies[testStrng]-5))		
				TestControl.motorStep(testStrng, 100, 0);	
			else if(freq < (stringFrequencies[testStrng]-1))		
				TestControl.motorStep(testStrng, 50, 0);	
			else if(freq < (stringFrequencies[testStrng]-.5))		
				TestControl.motorStep(testStrng, 25, 0);
			else if(freq < (stringFrequencies[testStrng]))		
				TestControl.motorStep(testStrng, 10, 0);
			


			sleep(4); //Let string settle
			prevFreq=freq;
			do{
				TestControl.sampleString(testStrng);
				freq=TestControl.getFreq(stringFrequencies[testStrng]);
			} while (freq==0);			
			cout<<"Frequency: "<<freq<<"  PrevFrequency:"<<prevFreq<<"  Attempt #"<<aindex<<endl;
			if(freq>=stringFrequencies[testStrng] || freq>stringFrequencies[testStrng]-.1)
				inTune = true;
			aindex++;
		
		}	
}


/****************************************************************** 
*	tuneUpv1 - Attempts to tune the string up to the proper pitch
*	by varying the number of steps taken based on the proximity 
*	with the desired frequency
******************************************************************/ 

void tuneUpv1(int testStrng)
{
	double tension=0;
	double freq=0;
	double prevFreq=0;
	bool inTune= false;
	int aindex=0;
	/****************************************************************** 
	 *	Tighten string up 2 pounds less than tension from
	 *      test configuration. Stop at less than 100 pounds
	 *      which is max load for loadcell. Do not tension if string
	 *      is broken.
	 *      Note: this code will most likely end up being performed by an
	 *  	      outside program to allow for plucking motor adjustments	
	 ******************************************************************/  
	/*	tension=TestControl.sampleTens(testStrng);
		cout<<"Desired Tension: "<<stringTensions[testStrng]<<endl;
		while(tension < (stringTensions[testStrng]-2)  && tension < 98 && stringBreak[testStrng] !=1)
		{	
			tension=TestControl.sampleTens(testStrng);		
			cout<<tension<<endl;
			if(tension < stringTensions[testStrng])
				TestControl.motorStep(testStrng, 500, 0);
			tension=TestControl.sampleTens(testStrng);		
			cout<<tension<<endl;
		}*/


	/****************************************************************** 
	 *	Get a good frequency and write it to file
	 ******************************************************************/  
		cout<<"Desired freq: "<<stringFrequencies[testStrng]<<endl;
		while(freq==0)
		{
			TestControl.sampleString(testStrng);
			freq=TestControl.getFreq(stringFrequencies[testStrng]);
		}
		cout<<"Initial freq: "<<freq<<endl;
		prevFreq = freq;
		ofstream fDat;
		fDat.open ("initialFreq.dat", ios::app);
		fDat<<freq<<endl;
		fDat.close();
	/****************************************************************** 
	 *	Begin tuning up the string.
	 *      
	 ******************************************************************/  
		while(!inTune)
		{
			
			/*These work with nylon string but will be different for metal*/			
			if(freq < (stringFrequencies[testStrng]-5))		
				TestControl.motorStep(testStrng, 100, 0);	
			else if(freq < (stringFrequencies[testStrng]-1))		
				TestControl.motorStep(testStrng, 50, 0);	
			else if(freq < (stringFrequencies[testStrng]-.5))		
				TestControl.motorStep(testStrng, 25, 0);
			else if(freq < (stringFrequencies[testStrng]))		
				TestControl.motorStep(testStrng, 10, 0);
			


			sleep(4); //Let string settle
			prevFreq=freq;
			do{
				TestControl.sampleString(testStrng);
				freq=TestControl.getFreq(stringFrequencies[testStrng]);
			} while (freq==0 && (freq< prevFreq -15 || freq> prevFreq+15));			
			cout<<"Frequency: "<<freq<<"  PrevFrequency:"<<prevFreq<<"  Attempt #"<<aindex<<endl;
			if(freq>=stringFrequencies[testStrng] || freq>stringFrequencies[testStrng]-.1)
				inTune = true;
			aindex++;
		
		}	
}





/****************************************************************** 
*	Load test information in from testconfig file
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
		
	for(int i=0; i<6; i++)
	{	
		cout<<"String "<<i<<"\tFrequency: "<<stringFrequencies[i]<<"   "<<"\tTension: "<<stringTensions[i]<<endl;
	}
	config.close();	
}

int main ()
{
	double freq;
	string readString;
	ofstream rawDat;
  	
	readTestCon();
	TestControl.initializePort();
	tuneUpv1(0);	
	tuneUpv1(1);
	tuneUpv1(2);

	rawDat.open ("elapsedTension.dat"); 
	TestControl.closePort();
	rawDat<<TestControl.getElapsedSteps(0)<<endl;
	rawDat<<TestControl.getElapsedSteps(1)<<endl;
	rawDat<<TestControl.getElapsedSteps(2)<<endl;
	rawDat.close();
	TestControl.closePort();
	
	
	return 0;
}


