
/*  Pulse Sensor Amped 1.4    by Joel Murphy and Yury Gitman   http://www.pulsesensor.com

----------------------  Notes ----------------------  ---------------------- 
This code:
1) Blinks an LED to User's Live Heartbeat   PIN 13
2) Fades an LED to User's Live HeartBeat
3) Determines BPM
4) Prints All of the Above to Serial

Read Me:
https://github.com/WorldFamousElectronics/PulseSensor_Amped_Arduino/blob/master/README.md   
 ----------------------       ----------------------  ----------------------
*/
#include <LiquidCrystal.h>


// select the pins used on the LCD panel
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

//  Variables
int pulsePin = 1;                 // Pulse Sensor purple wire connected to analog pin 1
int pulsePin2 = 2;                 // Pulse Sensor purple wire for second sensor analog pin 2
int blinkPin = 13;                // pin to blink led at each beat
int fadePin = 5;                  // pin to do fancy classy fading blink at each beat
int fadeRate = 0;                 // used to fade LED on with PWM on fadePin

// Volatile Variables, used in the interrupt service routine!
volatile int BPM;                   // int that holds raw Analog in 0. updated every 2mS
volatile int Signal;                // holds the incoming raw data
volatile int IBI = 600;             // int that holds the time interval between beats! Must be seeded! 
volatile boolean Pulse = false;     // "True" when User's live heartbeat is detected. "False" when not a "live beat". 
volatile boolean QS = false;        // becomes true when Arduoino finds a beat.


// Volatile Variables, used in the interrupt service routine!
volatile int BPM2;                   // int that holds raw Analog in 0. updated every 2mS
volatile int Signal2;                // holds the incoming raw data
volatile int IBI2 = 600;             // int that holds the time interval between beats! Must be seeded! 
volatile boolean Pulse2 = false;     // "True" when User's live heartbeat is detected. "False" when not a "live beat". 
volatile boolean QS2 = false;        // becomes true when Arduoino finds a beat.

volatile int differenceTime;

// Regards Serial OutPut  -- Set This Up to your needs
static boolean serialVisual = true;   // Set to 'false' by Default.  Re-set to 'true' to see Arduino Serial Monitor ASCII Visual Pulse 


void setup(){
  pinMode(blinkPin,OUTPUT);         // pin that will blink to your heartbeat!
  pinMode(fadePin,OUTPUT);          // pin that will fade to your heartbeat!
  Serial.begin(9600);             // we agree to talk fast!
  interruptSetup();                 // sets up to read Pulse Sensor signal every 2mS 
   // IF YOU ARE POWERING The Pulse Sensor AT VOLTAGE LESS THAN THE BOARD VOLTAGE, 
   // UN-COMMENT THE NEXT LINE AND APPLY THAT VOLTAGE TO THE A-REF PIN
//   analogReference(EXTERNAL);   

  lcd.begin(16, 2);              // start the library
  
}


//  Where the Magic Happens
void loop(){
  
   serialOutput() ;       
    
  if (QS == true){     // A Heartbeat Was Found
                       // BPM and IBI have been Determined
                       // Quantified Self "QS" true when arduino finds a heartbeat
        digitalWrite(blinkPin,HIGH);     // Blink LED, we got a beat. 
        fadeRate = 255;         // Makes the LED Fade Effect Happen
                                // Set 'fadeRate' Variable to 255 to fade LED with pulse
        serialOutputWhenBeatHappens();   // A Beat Happened, Output that to serial. 
        
        //Try printing out to second line of LCD here
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("BPM:"); // print a simple message
        lcd.setCursor(4,0);
        lcd.print(BPM);
        lcd.setCursor(8,0);
        lcd.print("BPM2:"); // print a simple message
        lcd.setCursor(13,0);
        lcd.print(BPM2);

        lcd.setCursor(0,1);
        lcd.print(differenceTime);
       
        QS = false;                      // reset the Quantified Self flag for next time    
  }


  if (QS2 == true){     // A Heartbeat Was Found
                       // BPM and IBI have been Determined
                       // Quantified Self "QS" true when arduino finds a heartbeat
        digitalWrite(blinkPin,HIGH);     // Blink LED, we got a beat. 
        fadeRate = 255;         // Makes the LED Fade Effect Happen
                                // Set 'fadeRate' Variable to 255 to fade LED with pulse
        serialOutputWhenBeatHappens2();   // A Beat Happened, Output that to serial. 
        
        //Try printing out to second line of LCD here
       lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("BPM:"); // print a simple message
        lcd.setCursor(4,0);
        lcd.print(BPM);
        lcd.setCursor(8,0);
        lcd.print("BPM2:"); // print a simple message
        lcd.setCursor(13,0);
        lcd.print(BPM2);

        lcd.setCursor(0,1);
        lcd.print(differenceTime);

        QS2 = false;                      // reset the Quantified Self flag for next time    
  }


  
     
  ledFadeToBeat();                      // Makes the LED Fade Effect Happen 
  delay(20);                             //  take a break
}





void ledFadeToBeat(){
    fadeRate -= 15;                         //  set LED fade value
    fadeRate = constrain(fadeRate,0,255);   //  keep LED fade value from going into negative numbers!
    analogWrite(fadePin,fadeRate);          //  fade LED
  }




