/*****************************************************************************
* Arduino Control Program for D'Addario String Test Device 
*
* This program interprets commands recieved via UART to control string
* stretch testing apparatus. Uses stringTestControls library.
* Created by Matt Fors
* Binghamton University
* mfors1@binghamton.edu
******************************************************************************/
#include "Arduino.h"
#include "stringTestControls.h"


stringTestControls TestControl;	 	
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

void setup()
{

	Serial.begin(CONTROLSBAUDE);  
	Serial.println("ARDUINO: Initialized");
	inputString.reserve(10); //Holds input command	
	
}

void loop()
{

  
	/**********************************************
	* COMMAND EXAMPLES:
	* TestControl.sampleFreq(0) Sample piezo port 0
	* TestControl.sampleTens(0,25) String 0, 25 Samples
	* TestControl.motorStep(2, 250,0) String2, 250 Steps,Direction 0
	* TestControl.pluck(1) String 1
	**********************************************/

	/****************************************************************** 
 	*	Command interpreter 
 	******************************************************************/ 
  	if (stringComplete) 
	{
		if(inputString[0] == '0') //Step
   		{
      			int steps = ((inputString[2]-48)*1000+(inputString[3]-48)*100 + (inputString[4]-48)*10 + (inputString[5]-48));
      			int dir =inputString[1]-48;
      			//int str=0; //For 1 String Demo
      			int str=inputString[6]-48; //For 6 String Setup
    		      	/******************************************* 
		       	*	1 Clock  Tighten
		       	*	0 Counter-Clockwise Loosen
		       	*******************************************/
			TestControl.motorStep(str, steps, dir);
                        Serial.print(str);
			Serial.print(" Done Step ");
                        Serial.println(steps);
      
    		}
    		else if(inputString[0] == '1') //Sample
    		{
			int strng =inputString[1]-48;
                        TestControl.sampleFreq(strng);
    		}
    		else if(inputString[0] == '2') //Loadcell
    		{
                        int strng =inputString[1]-48;
      			TestControl.sampleTens(strng); //For 1 String Demo
      			//TestControl.sampleTens((inputString[1]-48)); //6 String Setup
         
    		}
   		else if(inputString[0] == '3') //Pluck
    		{
			TestControl.pluck((inputString[1]-48));
			Serial.print("ARDUINO:Pluck ");
			Serial.println((inputString[1]-48));
      
    		}
     		else if(inputString[0] == '4') //Half Pluck
    		{
			TestControl.halfPluck((inputString[1]-48));
			Serial.print("ARDUINO:HalfPluck ");
			Serial.println((inputString[1]-48));
      		}
                
               
                
		inputString = "";
		stringComplete = false;
 	}
}

/****************************************************************** 
 *	Wait for a command. Fill a buffer untill an end line is reached  
 ******************************************************************/ 
void serialEvent() 
{
	while (Serial.available())
	{
		// get the new byte:
		char inChar = (char)Serial.read();
    		// add it to the inputString:
    		inputString += inChar;
    		// if the incoming character is a newline, set a flag
    		// so the main loop can do something about it:
   		if (inChar == '\n')
			{stringComplete = true;}
  	}
}




