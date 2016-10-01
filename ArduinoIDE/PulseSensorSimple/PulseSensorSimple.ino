/*
 *  Biodev
 *  Pulse Sensor Monitor
 *  October 2016
 */


#include <LiquidCrystal.h>


// select the pins used on the LCD panel
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);


int pulsePin = 1;

// Volatile Variables, used in the interrupt service routine!
volatile int BPM;                   // int that holds raw Analog in 0. updated every 2mS
volatile int Signal;                // holds the incoming raw data
volatile int IBI = 600;             // int that holds the time interval between beats! Must be seeded! 
volatile boolean Pulse = false;     // "True" when User's live heartbeat is detected. "False" when not a "live beat". 
volatile boolean QS = false;        // becomes true when Arduoino finds a beat.




void setup(){
  
  interruptSetup();               // sets up to read Pulse Sensor signal every 2mS  

  lcd.begin(16, 2);              // start the library
  lcd.setCursor(0,0);
  lcd.print("Analyzing");
  
}



void loop(){
        
    
  if (QS == true){     // A Heartbeat Was Found
                       // BPM and IBI have been Determined
                       // Quantified Self "QS" true when arduino finds a heartbeat
        //Try printing out to second line of LCD here
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Analyzing");
        lcd.setCursor(0,1);
        lcd.print("BPM:"); // print a simple message
        lcd.setCursor(6,1);
        lcd.print(BPM);
       
        QS = false;                      // reset the Quantified Self flag for next time    
  }
     
  delay(20);                        
}






