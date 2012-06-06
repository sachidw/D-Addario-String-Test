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
	double freq2;
	double tension;
	double freq;
	int strng =0;
	int totalS=0;
	int stepSize=0;
	char sSteps[4]; //Step size as a string	
	double lastFreq=0;	
	double slope=0;
	int lastStepSize=0;
	/****************************************************************** 
 	*	Determine an initial rate of change by getting the change in 
 	*	pitch between 400 steps
 	******************************************************************/  
		TestControl.sampleString(testStrng);
		lastFreq=TestControl.getFreq(stringFrequencies[strng]);
		TestControl.motorPower(1);	
		sleep(5);			
		TestControl.motorStep(testStrng, "400", "0");
		TestControl.motorPower(0);
		sleep(5); //Allow string to settle after stepping the motor
		TestControl.sampleString(testStrng);
		freq=TestControl.getFreq(stringFrequencies[strng]);
		cout<<"1st freq: "<<lastFreq<<"  2nd freq: "<<freq<<endl;	
		slope = (freq-lastFreq)/400.0;
		cout<<"INITIAL SLOPE: "<<slope<<endl;
	
	/****************************************************************** 
 	*	Approximate the number of steps need to read 2 Hz less than 
	*	the desired frequency base on rate of change.
 	*	
 	******************************************************************/  
	while( freq < stringFrequencies[strng]-1)
	{
		
		
		if(lastFreq > stringFrequencies[strng]-3 )
		{
			
			int done=0;
			while(done == 0)
			{		
				cout<<"CLOSE FREQ TEST"<<endl;
				TestControl.sampleString(testStrng);
				freq=TestControl.getFreq(stringFrequencies[strng]);
				cout<<"FREQ 1: "<<freq<<endl;
				sleep(3);
				TestControl.sampleString(testStrng);
				freq2=TestControl.getFreq(stringFrequencies[strng]);
				cout<<"FREQ 2: "<<freq2<<endl;
				cout<<"DIF: "<<abs(freq-freq2)<<endl;
				if( abs(freq-freq2) < .5  )
				{
					freq=freq2;
					done=1;
				}
				
			}
			cout<<"END OF CLOSE FREQ TEST"<<endl;
			
		}
		else
		{
			TestControl.sampleString(testStrng);
			freq=TestControl.getFreq(stringFrequencies[strng]);

		}		
		
		cout<<"FREQ: "<<freq<<endl;
		if(freq <(lastFreq+80) && freq>(lastFreq-100)  )
		{			
		 			if(lastStepSize==0) //First time in loop use initial slope
		 			{	
		 				stepSize = (((stringFrequencies[0]-1)-freq) / slope);
		 				cout<<"1ststepSize: "<<stepSize<<endl;	
		 				lastStepSize = stepSize;
		 			}		
		 			else //Calculate a new slope based on the frequency before stepping
		 			{		
		 				slope = (freq-lastFreq)/lastStepSize;
		 				cout<<"SLOPE: "<<slope<<endl;
		 				stepSize = (( (stringFrequencies[0]-1)   -freq) / slope);
		 				lastStepSize = stepSize;
		 				cout<<"stepSize: "<<stepSize<<endl;
		 			}
		
		
		
		
		 			cout<<stepSize<<"\t\t"<<freq<<"\t\t"<<slope<<endl;
		 			if(slope >0 && stepSize >0 && slope > .001)			
		 			{	
		 				TestControl.motorPower(1);	
		 				sleep(5);					
		 				while(stepSize > 999)
		 				{
									
		 					TestControl.motorStep(testStrng, "999", "0");
		 					stepSize= stepSize-999;
		 					cout<<"stepSize: "<<stepSize<<endl;
						} //while(stepSize > 999)
					
		 				/*Need to convert stepSize to a string*/					
		  
						sSteps[0]= ((stepSize - (stepSize%100))/100)+48;
		 				sSteps[1]= (   ((stepSize%100) - ((stepSize%100)%10)) / 10       )+48;
		 				sSteps[2]= (((stepSize%100)%10))+48;
						cout<<"sSteps: "<<sSteps<<endl;
						TestControl.motorStep(testStrng, sSteps, "0");
		 				TestControl.motorPower(0);
		 				sleep(5); //Allow string to settle after stepping the motor
		 				lastFreq=freq;
		 			}
		 			else
		 				cout<<"NEGATIVE SLOPE OR TOO SMALL"<<endl;
		 		}

		
		}
	

		/****************************************************************** 
 		*	Reach the final frequency by stepping by 20
 		*	
 		******************************************************************/  		
		//cout<<"END OF SLOPE LOOP___________________"<<endl;
		while( freq < stringFrequencies[strng])
		{
			TestControl.sampleString(testStrng);
			freq=TestControl.getFreq(stringFrequencies[strng]);			
			cout<<"FREQ: "<<freq<<endl;			
			if(freq <(lastFreq+15) && freq>(lastFreq-15) && freq < stringFrequencies[strng])
			{
				TestControl.motorPower(1);	
				sleep(5);	
				TestControl.motorStep(testStrng, "020", "0");
				TestControl.motorPower(0);
				sleep(5);
				lastFreq=freq;
			}
				
		}
		
		TestControl.motorPower(0);
		sleep(5);	
		TestControl.motorPower(0);
		TestControl.motorStep(6, "000", "0");
		
}





/****************************************************************** 
 *	This function tunes specified string to frequency outlined
 *	in the testconfig file	
 ******************************************************************/  
void tuneUp(int strng)
{
	double tension=0;
	double freq=0;
	int testStrng =strng;

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
	 *	Get a good frequency and write it to file
	 ******************************************************************/  
			cout<<"Desired freq: "<<stringFrequencies[strng]<<endl;
			double prevFreq=0;
			while(freq==0)
			{
				TestControl.sampleString(testStrng);
				freq=TestControl.getFreq(stringFrequencies[strng]);
			}
			cout<<"INITIAL FREQ: "<<freq<<endl;
			prevFreq = freq;
			ofstream fDat;
  			fDat.open ("fData.txt", ios::app);
			fDat<<freq<<endl;
			fDat.close();
	/****************************************************************** 
	 *	Begin tuning up the string.
	 *      
	 ******************************************************************/  
			cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Increment: 100"<<endl;
			//Increment by 100 steps until withing 4 Hz of desired frequency
				while(freq < (stringFrequencies[strng]-1) && stringBreak[strng] !=1)
				{		
					freq=0;					
					while(freq==0)
					{
						TestControl.sampleString(testStrng);
						freq=TestControl.getFreq(stringFrequencies[strng]);
						if(freq==0)
							TestControl.halfPluck(testStrng);				
					}
					
					cout<<"Frequency: "<<freq<<endl;
					cout<<"prevFreq: "<<prevFreq<<endl;
					//Ignore sample if it's not withing 15 Hz of previous test
					//assume data is bad
					if(freq < (stringFrequencies[strng]-1) && (freq <(prevFreq+15) && freq>(prevFreq-15))   )
					{			
						TestControl.motorPower(1);	
						sleep(5);						
						TestControl.motorStep(testStrng, "100", "0");	
						TestControl.motorPower(0);	
						sleep(5);
						prevFreq=freq;
					}

					/*else if(freq > (stringFrequencies[strng]-3) && (freq < (prevFreq+15) && freq> (prevFreq-15))   )
					{			
						prevFreq=freq;
					}*/

					else if (!(freq <prevFreq+15 && freq>prevFreq-15))
						freq=prevFreq;	
		
				}	
			//Increment by 50 steps until withing .5 Hz of desired frequency
				cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~Increment: 50"<<endl;
				while(freq < (stringFrequencies[strng])-.5 && stringBreak[strng] !=1)
				{		
					freq=0;					
					while(freq==0)
					{
						TestControl.sampleString(testStrng);
						freq=TestControl.getFreq(stringFrequencies[strng]);
						if(freq==0)
							TestControl.halfPluck(testStrng);				
					}
					cout<<"Frequency: "<<freq<<endl;
					cout<<"prevFreq: "<<prevFreq<<endl;
					//Ignore sample if it's not withing 10 Hz of previous test
					//assume data is bad
					if(freq < (stringFrequencies[strng]-.5) && (freq < (prevFreq+10) && freq> (prevFreq-10))   )
					{			
						TestControl.motorPower(1);
						sleep(5);							
						TestControl.motorStep(testStrng, "050", "0");
						TestControl.motorPower(0);
						sleep(5);	
						prevFreq=freq;
					}
		/*
					else if(freq > (stringFrequencies[strng]-1) && (freq < (prevFreq+10) && freq> (prevFreq-10))   )
					{			
						prevFreq=freq;
					}*/

					else if (!(freq <prevFreq+10 && freq>prevFreq-10))
						freq=prevFreq;	
		
		
				}	
			//Increment by 10 steps until arrive at desired frequency
				cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~`Increment: 20"<<endl;
				while(freq < (stringFrequencies[strng])-.1 && stringBreak[strng] !=1)
				{		
					freq=0;					
					while(freq==0)
					{
						TestControl.sampleString(testStrng);
						freq=TestControl.getFreq(stringFrequencies[strng]);
						if(freq==0)
							TestControl.halfPluck(testStrng);				
					}
					cout<<"Frequency: "<<freq<<endl;
					cout<<"prevFreq: "<<prevFreq<<endl;
					//Ignore sample if it's not withing .8 Hz of previous test
					//assume data is bad
					if(freq < (stringFrequencies[strng]) && (freq <(prevFreq+10) && freq>(prevFreq-10)))
					{			
						TestControl.motorPower(1);
						sleep(5);							
						TestControl.motorStep(testStrng, "020", "0");
						TestControl.motorPower(0);
						sleep(5);	
						prevFreq=freq;
					}

					/*else if(freq > (stringFrequencies[3]-3) && (freq < (prevFreq+.8) && freq> (prevFreq-.8))   )
					{			
						prevFreq=freq;
					}*/

					else if (!(freq <prevFreq+1 && freq>prevFreq-1))
						freq=prevFreq;		
		
		
				}	
	
	TestControl.motorPower(0);
	sleep(5);	
	TestControl.motorPower(0);
	TestControl.motorStep(6, "000", "0");
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
  	rawDat.open ("testData.txt"); 
	//stringFrequencies[0]=110.00;
	//stringTensions[0]=15;
	//stringFrequencies[1]=329.63;
	//stringTensions[1]=15;	
	readTestCon();
	TestControl.initializePort();
	sleep(2);
	/*
	TestControl.motorPower(1);
	sleep(5);
	TestControl.motorPower(0);
	sleep(5);
	TestControl.motorPower(1);
	sleep(5);
	TestControl.motorPower(0);
	*/
	//stringFrequencies[0]
	/*
	TestControl.sampleString(0);
	freq= TestControl.getFreq(stringFrequencies[0]);
	cout<<freq<<endl;
	*/
	/*
	if(freq==0)	
		TestControl.halfPluck(0);
	else
		cout<<freq<<endl;
	*/
		
	tuneUp(0);
	tuneUp(1);
	tuneUp(2);
	//tuneUpv2(1);
	rawDat<<TestControl.getElapsedSteps(0)<<endl;
	rawDat<<TestControl.getElapsedSteps(1)<<endl;
	rawDat<<TestControl.getElapsedSteps(2)<<endl;
	//tuneUpv2(1);
	/*
	TestControl.sampleString(1);
	freq= TestControl.getFreq(stringFrequencies[1]);
	if(freq==0)	
		TestControl.halfPluck(1);
	else
		cout<<freq<<endl;
	*/
	//TestControl.motorStep(0, "400", "0");
	//sleep(5); //Allow string to settle after stepping the motor
	//TestControl.sampleString(0);
	//cout<<TestControl.getFreq(200)<<endl;
	rawDat.close();
	TestControl.closePort();
	
	
	
	
	return 0;
}


