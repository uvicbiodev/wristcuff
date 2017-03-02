/*
 *  Biodev
 *  Pulse Sensor Monitor
 *  October 2016
 */


#include <LiquidCrystal.h>


// select the pins used on the LCD panel
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);


int pulsePin = 1;

// Volatile Variables, used in the interrupt service routine
volatile int BPM;                   // calculated heart rate, beats per minute
volatile int Signal;                // incoming raw data
volatile int IBI = 600;             // int that holds the time interval between beats 
volatile bool Pulse = false;
//volatile int powerPin = A2;
bool Flag = false;

void setup(){

  // initialize the serial communications:
  Serial.begin(9600);

  //pinMode(powerPin, OUTPUT);
  
  interruptSetup();               // sets up to read Pulse Sensor signal every 2mS  
  
  lcd.begin(16, 2);              // start the library
  lcd.setCursor(0,0);
  lcd.print("Analyzing");
  
}



void loop(){
 acceleration(); 

 if(Flag == true){ 
  //Try printing out to second line of LCD here
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Analyzing");
  lcd.setCursor(0,1);
  lcd.print("BPM:"); // print a simple message
  lcd.setCursor(6,1);
  lcd.print(BPM);
  Serial.print("BPM: ");
  Serial.print(BPM);
  Serial.print("\n");
 }

  if(Flag == false){
  Serial.print("moving");
  Serial.print("\n");
  }


  
  
  

 
  

  /*
  //Serial.print("Signal: ");
  Serial.print(Signal);                         // serial print serial for testing
  Serial.print("\n");
  Serial.print("\n");
  Serial.print(IBI);                         // serial print serial for testing
  Serial.print("\n");
  delay(2);
  */
  
       
  delay(100); 

  
}

