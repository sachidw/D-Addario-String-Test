/**********************************************************************
* PC Control Library For D'Addario String Test Device 
* stringPCControls.h - Library for controlling string stretch device
* Created by Matt Fors
***********************************************************************/


#ifndef stringPCControls_h
#define stringPCControls_h
#include <string>
#include <fftw3.h>
#include <SerialStream.h>
using namespace std;
using namespace LibSerial;

class stringPCControls
{
public:
	stringPCControls();
	//Constructor

	void sampleString(int strng);
	// Sample piezo on string strng
	
	double getFreq(double upperLimit);
	//return the frequency of stored sample

	double sampleTens(int strng);
	// Sample tension of specified string 
	// Return Tension

	void motorStep(int strng, string numSteps, string direction);
	// Step specified string numSteps in specified direction
	// The maximum step size is 999
	// Return 0 if no errors


	void pluck(int strng);
	// Pluck specified string
	
	void halfPluck(int strng);
	// Half Pluck specified string
	// In some cases the pluck motor stops at points
	// resting on the string. A half pluck will move
	// this resting point away from the string

	
	int getTotalSteps(int strng);
	//Return total number of steps take by specified string
	
	void resetPosition(int strng);
	//Resets motors to poistion at start of programk

	void initializePort();
	void closePort();
	int totalSteps[6];

	

private:
  	string readLine();
	
};

#endif

