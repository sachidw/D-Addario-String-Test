/**********************************************************************
* PC Control Library For D'Addario String Test Device 
* stringPCControls.cpp - Library for controlling string stretch device
* Created by Matt Fors
***********************************************************************/
#include <SerialStream.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <math.h>
#include <fftw3.h>
#include "stringPCControls.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>

//#include <fstream>

using namespace std;
using namespace LibSerial;
fftw_complex *in, *out;
fftw_plan p;		
double fs =1227; 	//Sampling Rate (Hz)
int N = 3600; 		//Number of samples
string PORT; 		//Arduino Port Location
SerialStream ardu;	//Arduino Stream



/****************************************************************** 
 *	stringPCControls constructor
 ******************************************************************/  	
stringPCControls::stringPCControls()
{
	/*FFTW Storage*/ 	
	in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N); 
	out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
	p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
	
	totalSteps[0]=0;
	totalSteps[1]=0;
	totalSteps[2]=0;
	totalSteps[3]=0;
	totalSteps[4]=0;
	totalSteps[5]=0;


}

/****************************************************************** 
 *	Reads bytes from serial stream until end of line is reached
 ******************************************************************/  
string stringPCControls::readLine()
{
	char next_byte;
	next_byte=NULL;
	string output;
	while(next_byte!='\n')
	{	
		ardu.get( next_byte ) ;
		output.append(1,next_byte);
	}
	return output;
	
}

/****************************************************************** 
 *	Initializes communication port with arduino
 ******************************************************************/  
void stringPCControls::initializePort()
{
	/****************************************************************** 
	*	Get port location from config file
	******************************************************************/  		
		string port;	
		ifstream config;
  		config.open ("config");
		getline(config, port);
		config.close();	
		cout<<"Port: "<<port<<endl;
	
	ardu.Open(port);
    	ardu.SetBaudRate(SerialStreamBuf::BAUD_57600);	
	ardu.SetCharSize(SerialStreamBuf::CHAR_SIZE_8);
	ardu.SetNumOfStopBits(1) ;
	ardu.SetParity( SerialStreamBuf::PARITY_NONE ) ;
	ardu.SetFlowControl( SerialStreamBuf::FLOW_CONTROL_NONE ) ;
	cout<<"Attempting Initialize Port"<<endl;	
	cout<<readLine(); //Arduino sends "Initialized"
}

void stringPCControls::closePort()
{
	ardu.Close() ;	
	cout<<"Port Closed"<<endl;
}


/****************************************************************** 
 *	Determine the fundamental pitch of the sample in storage
 *	Notes:
 *		(1)58 <-> 62 Is the 60Hz hum range to be ignored
 *		(2)upperlimit+10 frequencies are ignored as they will be 
 *		   harmonics of the string looking to be tuned
 *		(3)abs(out[i]) Gives you the magnitude at that particular bin
 *		(4)(i*(fs/N) Gives you the frequency for that bin  
 ******************************************************************/  

double stringPCControls::getFreq(double upperLimit)
{
	fftw_execute(p);
	int max = 1;		
	int max2 =1;
	int max3 =1;
	
	string readString;
	ofstream rawDat;
  	rawDat.open ("rawDat.txt"); //rawDat.txt will contain the FFT data after each call of this function
	
	//Find the location of the 3 maximum bins 
	for(int i =0; i <(N/2); i++)
	{
		rawDat<<i*(fs/N)<<"\t\t"<<abs(*out[i])<<endl;	//Send the magnitude and frequency data to a tab deliminated file		
				
		if(abs(*out[i]) > abs(*out[max]) && i!=0 && ((i*(fs/N))>62 || (i*(fs/N))<58) && (i*(fs/N)< upperLimit+10) )
		{	
			max3 = max2;
			max2 = max;
			max = i;
			//cout<<max*(fs/N)<<" "<< max2*(fs/N) <<" "<< max3*(fs/N)<<endl;

		}
		else if(abs(*out[i]) > abs(*out[max2]) && i!=0 && ((i*(fs/N))>62 || (i*(fs/N))<58) && (i*(fs/N)< upperLimit+10) )
		{	
			if( (i*(fs/N) < (max*(fs/N)-10)) || (i*(fs/N) > (max*(fs/N)+10))) 			
			{				
				max3 = max2;
				max2 = i;
				//cout<<max*(fs/N)<<" "<< max2*(fs/N)<<" "<< max3*(fs/N)<<endl;
			}
		}
		else if(abs(*out[i]) > abs(*out[max3]) && i!=0 && ((i*(fs/N))>62 || (i*(fs/N))<58) && (i*(fs/N)< upperLimit+10) )		
		{
			if(   (i*(fs/N) < (max*(fs/N)-10)) || (i*(fs/N) > (max*(fs/N)+10)) ||  (i*(fs/N) < (max2*(fs/N)-10)) || (i*(fs/N) > (max2*(fs/N)+10))      )
			{
				max3 = i;
				//cout<<max*(fs/N)<<" "<< max2*(fs/N) <<" "<< max3*(fs/N)<<endl;
			}
			
		}
	}
	/****************************************************************** 
 	*	Barycentric method for bin interpolation
	*		y1  = |X[k - 1]|
	*		y2 =  |X[k]|
	*		y3 =  |X[k + 1]|
	*		d = (y3 - y1) / (y1 + y2 + y3)
	*		k'  =  k + d
 	******************************************************************/  
		
		int y1 = abs(*out[max-1]);	
		int y2 = abs(*out[max]);
		int y3 = abs(*out[max+1]);
		double dmax = ((y3-y1)*1.0) / (y1+y2+y3);	

		int y12 = abs(*out[max2-1]);	
		int y22 = abs(*out[max2]);
		int y32 = abs(*out[max2+1]);
		double dmax2 = ((y32-y12)*1.0) / (y12+y22+y32);	

		int y13 = abs(*out[max3-1]);	
		int y23 = abs(*out[max3]);
		int y33 = abs(*out[max3+1]);
		double dmax3 = ((y33-y13)*1.0) / (y13+y23+y33);


	rawDat.close();
	return (max*(fs/N)+dmax);
}

/****************************************************************** 
 *	Requests a sample set and put it into FFTW plan 
 ******************************************************************/  
void stringPCControls::sampleString(int selectedString)
{
	string testString;
	string strng;
	if(selectedString==5)
		strng="5";
	else if(selectedString==4)
		strng="4";
	else if(selectedString==3)
		strng="3";
	else if(selectedString==2)
		strng="2";
	else if(selectedString==1)
		strng="1";
	else
		strng="0";
	int index=0;	
	string command;
	command.append("1");
	command.append(strng);
	command.append("\n");	
	//cout<<"Begin Sampling"<<endl;	
	ardu << command;	
	//Read line until Arduino sends "END"
	while(testString[0]!='E')
	{
		testString= readLine();
		if(testString[0]!='E')		
		{		
			in[index][0] = atof(testString.c_str());
			in[index++][1] = 0.0; 
		}	
	}
	//cout<<"Done Sampling"<<endl;
}



/****************************************************************** 
 *	Requests 10 samples from loadcell and return the average 
 *	in pounds
 ******************************************************************/  
double stringPCControls::sampleTens(int selectedString)
{
		
	string recievedValue;	
	string strng;
	if(selectedString==5)
		strng="5";
	else if(selectedString==4)
		strng="4";
	else if(selectedString==3)
		strng="3";
	else if(selectedString==2)
		strng="2";
	else if(selectedString==1)
		strng="1";
	else
		strng="0";
	
	double sensorVolts; 	
	double sensorPounds;	
	double accum=0;	
	double conversion=100/3.0;
	
	string command;
	command.append("2");
	command.append(strng);
	command.append("\n");
	
	//Take 10 samples and average them together
	for(int i=0; i <10; i++)
	{
		ardu << command;
		recievedValue = readLine();
		sensorVolts = atof(recievedValue.c_str());	//Convert string into double
		sensorPounds=sensorVolts*(conversion);		//Convert Volts into pounds
		accum = accum +sensorPounds;
	}
	sensorPounds = accum/10.0;				//Take average of readings	
	return sensorPounds;
}
void stringPCControls::motorStep(int selectedString, string numSteps, string direct)
{
	  	
	string strng;
	string command;
	
		if(selectedString==5)
			strng="5";
		else if(selectedString==4)
			strng="4";
		else if(selectedString==3)
			strng="3";
		else if(selectedString==2)
			strng="2";
		else if(selectedString==1)
			strng="1";
		else
			strng="0";



	command.append("0");
	command.append(direct);
	command.append(numSteps);
	command.append(strng);
	command.append("\n");	
	ardu << command;
	readLine();
	//cout<<numSteps<<"  "<<readLine();
	if (strcmp (direct.c_str( ), "1") != 0)
	{	
		totalSteps[atoi(strng.c_str( ))]= totalSteps[atoi(strng.c_str( ))]   +    ( (numSteps[0]-48)*100 + (numSteps[1]-48)*10 + (numSteps[2]-48));
		//cout<<totalSteps[atoi(strng.c_str( ))];
	}	
	else if(strcmp (direct.c_str( ),"0") != 0)
	{	
		totalSteps[atoi(strng.c_str( ))]= totalSteps[atoi(strng.c_str( ))]   -    ( (numSteps[0]-48)*100 + (numSteps[1]-48)*10 + (numSteps[2]-48));
		//cout<<totalSteps[atoi(strng.c_str( ))];
	}
}

/****************************************************************** 
 *	Send command to pluck string 
 ******************************************************************/ 
void stringPCControls::pluck(int selectedString)
{
	
	string strng;
	string command;
		if(selectedString==5)
			strng="5";
		else if(selectedString==4)
			strng="4";
		else if(selectedString==3)
			strng="3";
		else if(selectedString==2)
			strng="2";
		else if(selectedString==1)
			strng="1";
		else
			strng="0";

	command.append("3");
	command.append(strng);
	command.append("\n");	
	ardu << command;
 	cout<<readLine();
}

/****************************************************************** 
 *	Send command to half pluck string 
 ******************************************************************/ 
void stringPCControls::halfPluck(int selectedString)
{
	
	string strng;
	string command;
		if(selectedString==5)
			strng="5";
		else if(selectedString==4)
			strng="4";
		else if(selectedString==3)
			strng="3";
		else if(selectedString==2)
			strng="2";
		else if(selectedString==1)
			strng="1";
		else
			strng="0";

	command.append("4");
	command.append(strng);
	command.append("\n");	
	ardu << command;
 	cout<<readLine();
}

/****************************************************************** 
 *	Return elapsed steps since program began 
 ******************************************************************/ 
int stringPCControls::getTotalSteps(int strng)
{
	return totalSteps[strng];
}

/****************************************************************** 
 *	Step back motor to original position 
 ******************************************************************/  

void stringPCControls::resetPosition(int strng)
{
	//int selectedString = atoi(strng.c_str()); 		
		

	while(totalSteps[strng]>=900)
	{
		
		cout<<"Step size 900 ";
		cout<<totalSteps[strng]<<endl;
		motorStep(strng, "900", "1");
	}	
	while(totalSteps[strng]>=100)
	{	
		
		cout<<"Step size 100 ";		
		cout<<totalSteps[strng]<<endl;
		motorStep(strng, "100", "1");
	}	
	
	while(totalSteps[strng]>=10)
	{		
		cout<<"Step size 10 ";
		cout<<totalSteps[strng]<<endl;
		motorStep(strng, "010", "1");
	
	}	
	while(totalSteps[strng]>=1)
	{		
		
		cout<<"Step size 1 ";
		cout<<totalSteps[strng]<<endl;
		motorStep(strng, "001", "1");
	}
	
}


