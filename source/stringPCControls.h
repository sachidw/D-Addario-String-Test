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

#define MAXSTEPS 24000 //Maximum number of steps allowed to be taken 

class stringPCControls
{
public:
	stringPCControls();
	//Constructor

	~stringPCControls();
	//Destructor

	void sampleString(int strng);
	// Sample piezo on strng
	
	double getFreq(double upperLimit);
	//return the frequency of stored sample

	double sampleTens(int strng);
	// Sample tension of specified string 
	// Return Tension

	double sampleTensFast(int strng);
	// Sample Tension without averaging


	void motorStep(int strng, int numSteps, int direction);
	// Step specified string numSteps in specified direction
	// The maximum step size is 9999
	// Return 0 if no errors


	void pluck(int strng);
	// Pluck specified string
	
	void halfPluck(int strng); //No longer needed
	// Half Pluck specified string
	// In some cases the pluck motor stops at points
	// resting on the string. A half pluck will move
	// this resting point away from the string

	
	int getTotalSteps(int strng);
	//Return total number of steps take on specified string

	int getElapsedSteps(int strng);
	//Return the number of steps elapsed since start of program

	
	void resetPosition(int strng);
	//Resets motors to poistion at start of program

	
	void initializePort();
	//Initilizes the port with the Aurdion

	void closePort();
	//Closes the Arduino port


	int totalSteps[6];	//Absalute position of motors
	int elapsedSteps[6]; 	//Steps since start of program

	

private:
  	string readLine();
	//Reads a line from serial 
	//Reads until end of line is recieved

	void writePositions();
	//Write absalute positions of motor to file
	
};

#endif

