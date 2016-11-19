
// Init pins
#define sclk 13
#define mosi 11
#define cs   10
#define dc   9
#define rst  8 

//TFT display libraries
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>

Adafruit_ST7735 tft = Adafruit_ST7735(cs, dc, rst);

int pulsePin = 0;

// Volatile Variables, used in the interrupt service routine!
volatile int BPM;                   // int that holds raw Analog in 0. updated every 2mS
volatile int Signal;                // holds the incoming raw data
volatile int IBI = 600;             // int that holds the time interval between beats! Must be seeded! 
volatile boolean Pulse = false;     // "True" when User's live heartbeat is detected. "False" when not a "live beat". 
volatile boolean QS = false;        // becomes true when Arduoino finds a beat.

int AverageBPM[10] = {70};
int SummedAverage;


void setup(){
 
  Serial.begin(9600);             // we agree to talk fast!
  interruptSetup();                 // sets up to read Pulse Sensor signal every 2mS 


  tft.initR(INITR_GREENTAB); // initialize a ST7735R chip, green tab
  tft.fillScreen(ST7735_BLACK);

  Serial.println("Splash Title");
  tft.setTextSize(2);
  tft.setCursor(0, 10);
  tft.setTextColor(ST7735_WHITE);
  tft.print("BioDEV");
  tft.setCursor(76, 10);
  tft.setTextColor( tft.Color565( 0xFF, 0x77, 0x00) );
  tft.print("DEMO");
  delay(2000);

  Serial.println("Drawing biodev logo");
  tft.drawCircle(60,90,50,tft.Color565( 0xFF, 0x77, 0x00));
  tft.drawCircle(60,90,51,tft.Color565( 0xFF, 0x77, 0x00));
  tft.drawCircle(60,90,52,tft.Color565( 0xFF, 0x77, 0x00));
  tft.drawCircle(60,90,53,tft.Color565( 0xFF, 0x77, 0x00));
  tft.fillRect(10,85,100,10, tft.Color565( 0xFF, 0x77, 0x00));
  tft.fillRect(55,40,10,100, tft.Color565( 0xFF, 0x77, 0x0));
  delay(4000);

  // Clear screen
  Serial.println("Clear screen with BLACK");
  tft.fillScreen(ST7735_BLACK);

  //main data output screen
  tft.setTextSize(1);
  tft.fillRoundRect( 14,16, 100, 100, 8, tft.Color565( 0xC4, 0xC4, 0xC4) );
  tft.drawRoundRect( 14,16, 100, 100, 8, tft.Color565( 0x99, 0x99, 0x99) );
  // Title
  tft.fillRoundRect( 20,8, 60, 20, 4, tft.Color565( 0xFF, 0, 0) );
  tft.drawRoundRect( 20,8, 60, 20, 4, tft.Color565( 0x99, 0x99, 0x99) );
  // Title text
  tft.setCursor(28, 14);
  tft.setTextColor(ST7735_WHITE);
  tft.print("BPM data");
  // Message
  tft.setTextColor(ST7735_RED);
  tft.setCursor(22, 38);
  tft.print("Current : 72");
  tft.setCursor(22, 48);
  tft.print("Average: 75");
  tft.setCursor(22, 68);
  tft.print("normal rate");
  tft.setCursor(22, 78);
  tft.print("no concern");
}



void loop(){


    
  if (QS == true){     // A Heartbeat Was Found
                       // BPM and IBI have been Determined
                       // Quantified Self "QS" true when arduino finds a heartbeat

     
      //FIX THE SUMMED AVERAGE METHOD
      
      SummedAverage = 0;

      for(int i =0; i < 9; i++){
        AverageBPM[i] = AverageBPM[i+1];
        SummedAverage = SummedAverage + AverageBPM[i+1];
      }
      AverageBPM[9] = BPM;
      SummedAverage = BPM;

      SummedAverage = SummedAverage/10;

      
      String bpmVal = String(BPM);
      String AveBPM = String(SummedAverage);
       
      //clears the current bpm reading 
      tft.fillRect(72,37,30,10,tft.Color565( 0xC4, 0xC4, 0xC4));

      tft.setCursor(22, 38);
      tft.print("Current : " + bpmVal);


      tft.fillRect(72,47,30,10,tft.Color565( 0xC4, 0xC4, 0xC4));

      tft.setCursor(22, 48);
      tft.print("Average: " + AveBPM);

    
      QS = false;                      // reset the Quantified Self flag for next time    
  }
     
  
  delay(20);                             //  take a break
}







