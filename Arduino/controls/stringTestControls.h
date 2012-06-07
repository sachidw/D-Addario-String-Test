/**********************************************************************
* Arduino Control Library For D'Addario String Test Device 
* stringTestControls.h - Library for controlling string stretch device
* Created by Matt Fors
***********************************************************************/


#ifndef stringTestControls_h
#define stringTestControls_h

#include <Arduino.h>
/***********************************************
* Pin Definitions
***********************************************/

//Analog
	#define PIEZOPORT A0
	#define LOADCELLPORT0 A15
	#define LOADCELLPORT1 A14
	#define LOADCELLPORT2 A13
	#define LOADCELLPORT3 A9
	#define LOADCELLPORT4 A10
	#define LOADCELLPORT5 A11

//Digital
	#define SOLENOIDPORT0 22
	//#define SOLENOIDPORT1 44
	#define SOLENOIDPORT1 31
        #define SOLENOIDPORT2 30
	#define SOLENOIDPORT3 45
	#define SOLENOIDPORT4 3
	#define SOLENOIDPORT5 38

	#define STEPDIRECTIONPORT 11
	#define STEPSTEPPORT 12
        #define ENABLE 10

	#define MOTORSELECT0A 37
	#define MOTORSELECT0B 29
	#define MOTORSELECT1A 36
	#define MOTORSELECT1B 28
	#define MOTORSELECT2A 35
	#define MOTORSELECT2B 27
	#define MOTORSELECT3A 34
	#define MOTORSELECT3B 26
	#define MOTORSELECT4A 33
	#define MOTORSELECT4B 25
	#define MOTORSELECT5A 32
	#define MOTORSELECT5B 24

        #define MOTORPOWER 23
/************************************************/
#define LOADCELLPRECISION 4
#define STEPDELAY 600		//Delay between each step the motor
//#define SOLENOIDDELAY 80
//#define SOLENOIDDELAY 500 //Delay for motor
//#define SOLENOIDDELAY 2170//Delay for NEW motor
//#define SOLENOIDDELAY 2200//Delay for NEW motor May 22
#define SOLENOIDDELAY 2140//Delay for NEW motor May 29
#define SOLENOIDDELAY0 2140//Delay for NEW motor May 29
#define SOLENOIDDELAY1 3000//Delay for NEW motor May 29


#define CONTROLSBAUDE 57600	//Baud rate of UART
//#define CONTROLSBAUDE 38400
#define NSAMPLES 3600		//Number of samples to be taken



class stringTestControls
{
public:
  stringTestControls();
  //Constructor

  void sampleFreq(int numSamples);
  // Sample piezo for numSamples
  // Transmit data over USB


  int sampleTens(int strng);
  // Sample specified string 
  // Transmit data over USB
  // Return 0 if no errors

    int motorStep(int strng, int numSteps, int direction);
  // Step specified string numSteps in specified direction
  // Return 0 if no errors


  int pluck(int strng);
  // Pluck specified string
  // Return 0 if no errors
  
  int halfPluck(int strng);
  // Pluck specified string for half delay
  // Return 0 if no errors
  /*This is to move the motor's resting point
   Over time the motor may finish at a point where the pick is resting on
    on the string this half pluck is needed to move the resting position*/

  int motorStepDemo(int strng, int numSteps, int direction);
  // Step for 1 string demo
  // Return 0 if no errors


private:
  void printDouble( double val, byte precision);
  int lastString;
  // prints val with number of decimal places determine by precision

};

#endif

