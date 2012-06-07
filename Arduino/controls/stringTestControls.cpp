/**********************************************************************
 * Arduino Control Library For D'Addario String Test Device 
 * stringTestControls.cpp - Library for controlling string stretch device
 * Created by Matt Fors
 ***********************************************************************/

#include "Arduino.h"
#include "stringTestControls.h"


/****************************************************************** 
 *	Initialize ports on the Arduino 
 *	
 ******************************************************************/  
stringTestControls::stringTestControls()
{
	
	//Analog Pins	
	pinMode(PIEZOPORT, INPUT);
	pinMode(LOADCELLPORT0, INPUT);
	pinMode(LOADCELLPORT1, INPUT);
	pinMode(LOADCELLPORT2, INPUT);
	//pinMode(LOADCELLPORT3, INPUT);
	//pinMode(LOADCELLPORT4, INPUT);
	//pinMode(LOADCELLPORT5, INPUT);

	//Digital Pins
	pinMode(SOLENOIDPORT0, OUTPUT);
	pinMode(SOLENOIDPORT1, OUTPUT); 
	pinMode(SOLENOIDPORT2, OUTPUT); 
	pinMode(SOLENOIDPORT3, OUTPUT); 
	pinMode(SOLENOIDPORT4, OUTPUT); 
	pinMode(SOLENOIDPORT5, OUTPUT);
	
	pinMode(STEPSTEPPORT, OUTPUT);
	pinMode(STEPDIRECTIONPORT, OUTPUT);

	pinMode(MOTORSELECT0A, OUTPUT);
        digitalWrite(MOTORSELECT0A, LOW);
	pinMode(MOTORSELECT0B, OUTPUT);
         digitalWrite(MOTORSELECT0B, LOW);
	pinMode(MOTORSELECT1A, OUTPUT);
	pinMode(MOTORSELECT1B, OUTPUT);
	pinMode(MOTORSELECT2A, OUTPUT);
	pinMode(MOTORSELECT2B, OUTPUT);
	//pinMode(MOTORSELECT3A, OUTPUT);
	//pinMode(MOTORSELECT3B, OUTPUT);
	//pinMode(MOTORSELECT4A, OUTPUT);
	//pinMode(MOTORSELECT4B, OUTPUT);
	//pinMode(MOTORSELECT5A, OUTPUT);
	//pinMode(MOTORSELECT5B, OUTPUT);
        pinMode(MOTORPOWER, OUTPUT);
        pinMode(ENABLE, OUTPUT);
        lastString=6;
}

/****************************************************************** 
 *	Sample the piezo port for NSAMPLES. Convert to 0 centered 
 *	voltage and sent over UART. Send the string "END" after
 *	all samples are sent.
 *	
 ******************************************************************/ 
void stringTestControls::sampleFreq(int strng)
{
	int sensorValue[NSAMPLES];
	double sensorValVolts;
	pluck(strng);
  	
	//Fill a buffer with sample data
	for(int i =0; i <NSAMPLES; i++)
	{
		sensorValue[i] = analogRead(PIEZOPORT);//100 microseconds
		//                              Sampling Rate	
		//delay(2);			//472.8Hz
		//delay(1);			//888 Hz
		//delayMicroseconds(750);	//1142.8 Hz
		delayMicroseconds(700);		//1215 Hz 
		//delayMicroseconds(500);	//1606 Hz

	}

	for(int i =0; i <NSAMPLES; i++)
	{
		//Convert the sensorValue to volts and center it at 0
		sensorValVolts = (((sensorValue[i]/1023.0)*5.0)-2.5);
		printDouble(sensorValVolts,1);
	}
	Serial.println("END");
}




int stringTestControls::sampleTens(int strng)
{
  
	  int sensorValue;
	  double sensorValueVolts;

	  if(strng == 0)
	  {
	    sensorValue = analogRead(LOADCELLPORT0);	
	  }
	  else if(strng ==1)
	  {
	    sensorValue = analogRead(LOADCELLPORT1);
	  }
	  else if(strng ==2)
	  {
	    sensorValue = analogRead(LOADCELLPORT2);
	  }
	  else if(strng ==3)
	  {
	    sensorValue = analogRead(LOADCELLPORT3);
	  }
	  else if(strng ==4)
	  {
	    sensorValue = analogRead(LOADCELLPORT4);
	  }
	  else if(strng ==5)
	  {
	    sensorValue = analogRead(LOADCELLPORT5);
	  }
	  else
	  {
	    return 1;
	  }
  sensorValueVolts=((sensorValue/1023.0)*5.00);
  
  Serial.println(sensorValueVolts);
  //printDouble(sensorValueVolts, LOADCELLPRECISION);
  return 0;
}


int stringTestControls::motorStep(int strng, int numSteps, int direct)
{
	//Direction Selection
	if(direct ==1)
		digitalWrite(STEPDIRECTIONPORT, HIGH);  
	else
		digitalWrite(STEPDIRECTIONPORT, LOW);

	//Turn on the relays for slected motor
	if(strng == 0) //000
	{
                
		if(lastString != 0)
                {
                digitalWrite(MOTORSELECT0A, HIGH);
                //delay(2000);
		digitalWrite(MOTORSELECT0B, HIGH);
            	
                digitalWrite(MOTORSELECT1A, LOW);
		digitalWrite(MOTORSELECT1B, LOW);	
		digitalWrite(MOTORSELECT2A, LOW);
		digitalWrite(MOTORSELECT2B, LOW);	
		digitalWrite(MOTORSELECT3A, LOW);
		digitalWrite(MOTORSELECT3B, LOW);	
		digitalWrite(MOTORSELECT4A, LOW);
		digitalWrite(MOTORSELECT4B, LOW);
		digitalWrite(MOTORSELECT5A, LOW);
		digitalWrite(MOTORSELECT5B, LOW);
                }
                //lastString =0;
                
		
	}
	else if(strng ==1)//001
	{
                 if(lastString != 1)
                {
	        digitalWrite(MOTORSELECT0A, LOW);
		digitalWrite(MOTORSELECT0B, LOW);	
            
                digitalWrite(MOTORSELECT1A, HIGH);
               	digitalWrite(MOTORSELECT1B, HIGH);	

                digitalWrite(MOTORSELECT2A, LOW);
		digitalWrite(MOTORSELECT2B, LOW);	
		digitalWrite(MOTORSELECT3A, LOW);
		digitalWrite(MOTORSELECT3B, LOW);	
		digitalWrite(MOTORSELECT4A, LOW);
		digitalWrite(MOTORSELECT4B, LOW);
		digitalWrite(MOTORSELECT5A, LOW);
		digitalWrite(MOTORSELECT5B, LOW);
                }
                //lastString =1;
	     
        }
	else if(strng ==2)//010
	{
                if(lastString != 2)
                {
		digitalWrite(MOTORSELECT0A, LOW);
		digitalWrite(MOTORSELECT0B, LOW);
                digitalWrite(MOTORSELECT1A, LOW);
		digitalWrite(MOTORSELECT1B, LOW);
  
  
                digitalWrite(MOTORSELECT2A, HIGH);
                digitalWrite(MOTORSELECT2B, HIGH);

                digitalWrite(MOTORSELECT3A, LOW);
		digitalWrite(MOTORSELECT3B, LOW);	
		digitalWrite(MOTORSELECT4A, LOW);
		digitalWrite(MOTORSELECT4B, LOW);
		digitalWrite(MOTORSELECT5A, LOW);
		digitalWrite(MOTORSELECT5B, LOW);
                }	
                //lastString =2;
	}
	else if(strng ==3)//011
	{
                if(lastString != 3)
                {
		digitalWrite(MOTORSELECT0A, LOW);
		digitalWrite(MOTORSELECT0B, LOW);
                digitalWrite(MOTORSELECT1A, LOW);
		digitalWrite(MOTORSELECT1B, LOW);
                digitalWrite(MOTORSELECT2A, LOW);
		digitalWrite(MOTORSELECT2B, LOW);
                
                digitalWrite(MOTORSELECT3A, HIGH);
                //delay(2000);
		digitalWrite(MOTORSELECT3B, HIGH);

                digitalWrite(MOTORSELECT4A, LOW);
		digitalWrite(MOTORSELECT4B, LOW);
		digitalWrite(MOTORSELECT5A, LOW);
		digitalWrite(MOTORSELECT5B, LOW);
                }
                //lastString =3;	
	}
	else if(strng ==4)//100
	{
                if(lastString != 4)
                {
		digitalWrite(MOTORSELECT0A, LOW);
		digitalWrite(MOTORSELECT0B, LOW);
                digitalWrite(MOTORSELECT1A, LOW);
		digitalWrite(MOTORSELECT1B, LOW);
                digitalWrite(MOTORSELECT2A, LOW);
		digitalWrite(MOTORSELECT2B, LOW);	
                digitalWrite(MOTORSELECT3A, LOW);
		digitalWrite(MOTORSELECT3B, LOW);
                digitalWrite(MOTORSELECT4A, HIGH);
                //delay(2000);
		digitalWrite(MOTORSELECT4B, HIGH);
                digitalWrite(MOTORSELECT5A, LOW);
		digitalWrite(MOTORSELECT5B, LOW);
                }
                //lastString =4;
	}
	else if(strng ==5)//101
	{
                if(lastString != 5)
                {
	        digitalWrite(MOTORSELECT0A, LOW);
		digitalWrite(MOTORSELECT0B, LOW);
		digitalWrite(MOTORSELECT1A, LOW);
		digitalWrite(MOTORSELECT1B, LOW);	
		digitalWrite(MOTORSELECT2A, LOW);
		digitalWrite(MOTORSELECT2B, LOW);	
		digitalWrite(MOTORSELECT3A, LOW);
		digitalWrite(MOTORSELECT3B, LOW);	
		digitalWrite(MOTORSELECT4A, LOW);
		digitalWrite(MOTORSELECT4B, LOW);	
          
                digitalWrite(MOTORSELECT5A, HIGH);
                //delay(2000);
		digitalWrite(MOTORSELECT5B, HIGH);
                }
                //lastString =5;	
	}
        else if(strng ==6)
	{
		if (lastString ==0)
                {
                      digitalWrite(MOTORSELECT0A, LOW);
                      digitalWrite(MOTORSELECT0B, LOW);
		}
                else if (lastString ==1)
                {
                   digitalWrite(MOTORSELECT1A, LOW);
                   digitalWrite(MOTORSELECT1B, LOW);   
                           
                                
		}
                  else if (lastString ==2)
                {
                    digitalWrite(MOTORSELECT2A, LOW);
                    digitalWrite(MOTORSELECT2B, LOW);  
                           
                                
		}
                  else if (lastString ==3)
                {
                   digitalWrite(MOTORSELECT3A, LOW);
                   digitalWrite(MOTORSELECT3B, LOW);   
                           
                                
		}
                  else if (lastString ==4)
                {
                  digitalWrite(MOTORSELECT4A, LOW);
                  digitalWrite(MOTORSELECT4B, LOW);    
                           
                                
		}
                  else if (lastString ==5)
                {
                   digitalWrite(MOTORSELECT5A, LOW);
                   digitalWrite(MOTORSELECT5B, LOW);   
                           
                                
		}

                lastString =6;
            	
	}	
  
        else
		return 1;

        
	///Step the motor for numSteps
        digitalWrite(ENABLE, HIGH);
	for(int i =0; i <numSteps; i++)
	{
		digitalWrite(STEPSTEPPORT, HIGH);
		//delay(STEPDELAY);	
                delayMicroseconds(STEPDELAY);
		digitalWrite(STEPSTEPPORT, LOW);
		//delay(STEPDELAY);
                delayMicroseconds(STEPDELAY);	
	}
		 digitalWrite(ENABLE, LOW);
                //Turn off all the motor slection relays after done stepping
		digitalWrite(MOTORSELECT0A, LOW);
		digitalWrite(MOTORSELECT0B, LOW);
		digitalWrite(MOTORSELECT1A, LOW);
		digitalWrite(MOTORSELECT1B, LOW);	
		digitalWrite(MOTORSELECT2A, LOW);
		digitalWrite(MOTORSELECT2B, LOW);	
		digitalWrite(MOTORSELECT3A, LOW);
		digitalWrite(MOTORSELECT3B, LOW);	
		digitalWrite(MOTORSELECT4A, LOW);
		digitalWrite(MOTORSELECT4B, LOW);
		digitalWrite(MOTORSELECT5A, LOW);
		digitalWrite(MOTORSELECT5B, LOW);	


        
	return 0;

}

int stringTestControls::motorStepDemo(int strng, int numSteps, int direct)
{

  if(direct ==1)
  {
    //Serial.print("Counter-Clockwise, NumSteps: ");
    digitalWrite(STEPDIRECTIONPORT, HIGH);  
  }
  else
  {
    digitalWrite(STEPDIRECTIONPORT, LOW);
    //Serial.print("Clockwise, NumSteps: ");
  }
  //Serial.println(numSteps); 
  for(int i =0; i <numSteps; i++)
  {
    digitalWrite(STEPSTEPPORT, HIGH);
    //delay(STEPDELAY);	
    delayMicroseconds(STEPDELAY);
     digitalWrite(STEPSTEPPORT, LOW);
    //delay(STEPDELAY);
    delayMicroseconds(STEPDELAY);	
  }

}

/****************************************************************** 
 *	Perform the plucking action  
 ******************************************************************/  

int stringTestControls::pluck(int strng)
{
	  int sensorValue;
	  double sensorValVolts;
          //Turn specific solenoid for a delay period and turn off
	  if(strng == 0)
	  {
	    
            digitalWrite(SOLENOIDPORT0, HIGH);   
            delay(500);
	    sensorValue = analogRead(PIEZOPORT);//100 microseconds
            sensorValVolts = (((sensorValue/1023.0)*5.0)-2.5);
             
             while(sensorValVolts<3) 
            {
              sensorValue = analogRead(PIEZOPORT);//100 microseconds
            sensorValVolts = (((sensorValue/1023.0)*5.0));
              //Serial.println(sensorValVolts);  
              //delay(500);
            }
            
            //delay(SOLENOIDDELAY);              
	    digitalWrite(SOLENOIDPORT0, LOW);
            return 0;
	  }
	  else if(strng ==1)
	  {
        
	     digitalWrite(SOLENOIDPORT1, HIGH);   
            delay(500);
	    sensorValue = analogRead(PIEZOPORT);//100 microseconds
            sensorValVolts = (((sensorValue/1023.0)*5.0)-2.5);
           
            while(sensorValVolts<3) 
            {
              sensorValue = analogRead(PIEZOPORT);//100 microseconds
            sensorValVolts = (((sensorValue/1023.0)*5.0));
              //Serial.println(sensorValVolts);  
              //delay(500);
            }
            
            //delay(SOLENOIDDELAY);              
	    digitalWrite(SOLENOIDPORT1, LOW);
  
  /*
	    digitalWrite(SOLENOIDPORT1, HIGH);   
	    delay(SOLENOIDDELAY1);              
	    digitalWrite(SOLENOIDPORT1, LOW);*/
	    return 0;
	  }
	  else if(strng ==2)
	  {
	     digitalWrite(SOLENOIDPORT2, HIGH);   
            delay(500);
	    sensorValue = analogRead(PIEZOPORT);//100 microseconds
            sensorValVolts = (((sensorValue/1023.0)*5.0)-2.5);
            while(sensorValVolts<3) 
            {
              sensorValue = analogRead(PIEZOPORT);//100 microseconds
            sensorValVolts = (((sensorValue/1023.0)*5.0));
              //Serial.println(sensorValVolts);  
              //delay(500);
            }
            
            //delay(SOLENOIDDELAY);              
	    digitalWrite(SOLENOIDPORT2, LOW);
	    return 0;
	  }
	  else if(strng ==3)
	  {
	    digitalWrite(SOLENOIDPORT3, HIGH);   
	    delay(SOLENOIDDELAY);              
	    digitalWrite(SOLENOIDPORT3, LOW);
	    return 0;
	  }
	  else if(strng ==4)
	  {
	    digitalWrite(SOLENOIDPORT4, HIGH);   
	    delay(SOLENOIDDELAY);              
	    digitalWrite(SOLENOIDPORT4, LOW);
	    return 0;
	  }
	  else if(strng ==5)
	  {
	    digitalWrite(SOLENOIDPORT5, HIGH);   
	    delay(SOLENOIDDELAY);              
	    digitalWrite(SOLENOIDPORT5, LOW);
	    return 0;
	  }
	  else
	  return 1;

}

/****************************************************************** 
 *	Perform a half plucking action to move the motor incase
 *      it gets stuck resting on the string 
 ******************************************************************/  
int stringTestControls::halfPluck(int strng)
{
      //Turn specific solenoid for a delay period and turn off
	  if(strng == 0)
	  {
	    digitalWrite(SOLENOIDPORT0, HIGH);   
	    delay(SOLENOIDDELAY/2);              
	    digitalWrite(SOLENOIDPORT0, LOW);
	    return 0;
	  }
	  else if(strng ==1)
	  {
	    digitalWrite(SOLENOIDPORT1, HIGH);   
	    delay(SOLENOIDDELAY/2);              
	    digitalWrite(SOLENOIDPORT1, LOW);
	    return 0;
	  }
	  else if(strng ==2)
	  {
	    digitalWrite(SOLENOIDPORT2, HIGH);   
	    delay(SOLENOIDDELAY/2);              
	    digitalWrite(SOLENOIDPORT2, LOW);	
	    return 0;
	  }
	  else if(strng ==3)
	  {
	    digitalWrite(SOLENOIDPORT3, HIGH);   
	    delay(SOLENOIDDELAY/2);              
	    digitalWrite(SOLENOIDPORT3, LOW);
	    return 0;
	  }
	  else if(strng ==4)
	  {
	    digitalWrite(SOLENOIDPORT4, HIGH);   
	    delay(SOLENOIDDELAY/2);              
	    digitalWrite(SOLENOIDPORT4, LOW);
	    return 0;
	  }
	  else if(strng ==5)
	  {
	    digitalWrite(SOLENOIDPORT5, HIGH);   
	    delay(SOLENOIDDELAY/2);              
	    digitalWrite(SOLENOIDPORT5, LOW);
	    return 0;
	  }
	  else
	  return 1;

  
}
/****************************************************************** 
 *	Used for sending a double of various percisions   
 ******************************************************************/ 
void stringTestControls::printDouble( double val, byte precision)
{
  // prints val with number of decimal places determine by precision
  // precision is a number from 0 to 6 indicating the desired decimial places
  // example: lcdPrintDouble( 3.1415, 2); // prints 3.14 (two decimal places)

  if(val < 0.0)
  {
    Serial.print('-');
    val = -val;
  }	

  Serial.print (int(val)); //prints the int part
  if( precision > 0) {
    Serial.print("."); // print the decimal point
    unsigned long frac;
    unsigned long mult = 1;
    byte padding = precision -1;
    while(precision--)
      mult *=10;

    if(val >= 0)
      frac = (val - int(val)) * mult;
    else
      frac = (int(val)- val ) * mult;

    unsigned long frac1 = frac;
    while( frac1 /= 10 )
      padding--;
    while( padding--)
      Serial.print("0");
    Serial.println(frac,DEC) ;

  }
}




