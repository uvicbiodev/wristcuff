// BioDEV Display Final UI Testing
// March 2017


// Init Display pins
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


// Vitals variables
int BPM;                    // beats per minute
int SPO2;                   // saturation of peripheral oxygen
int systolic;               // systolic blood pressure
int diastolic;              // diastolic blood pressure
boolean bluetooth = false;  // bluetooth boolean true=connected false=otherwise
int batteryPercent;         // battery percentage (0-100)

void setup(){
 
  Serial.begin(9600);      
  
  tft.initR(INITR_GREENTAB);        // initialize a ST7735R chip, green tab
  tft.setRotation(1);               // rotate the screen 90 degrees
  tft.fillScreen(ST7735_BLACK);     // fill screen black

  SplashScreen();                   // display the splashscreen
  delay(2000);
  MainUI();                         // display the main output UI
  
}


void SplashScreen(){
  // Main splash screen outputed on startup
  
  Serial.println("Splash Title");
  tft.setTextSize(2);
  tft.setCursor(15, 10);
  tft.setTextColor(ST7735_WHITE);
  tft.print("BioDev");
  tft.setTextColor( tft.Color565( 0xFF, 0x77, 0x00) );
  tft.print(" DEMO");
  delay(2000);

  Serial.println("Drawing biodev logo");
  tft.drawCircle(80,80,40,tft.Color565( 0xFF, 0x77, 0x00));
  tft.drawCircle(80,80,41,tft.Color565( 0xFF, 0x77, 0x00));
  tft.drawCircle(80,80,42,tft.Color565( 0xFF, 0x77, 0x00));
  tft.drawCircle(80,80,43,tft.Color565( 0xFF, 0x77, 0x00));
  tft.drawCircle(80,80,44,tft.Color565( 0xFF, 0x77, 0x00));
  tft.fillRect(40,75,80,10, tft.Color565( 0xFF, 0x77, 0x00));
  tft.fillRect(75,40,10,80, tft.Color565( 0xFF, 0x77, 0x0));
  delay(4000);


  // Clear screen
  Serial.println("Clear screen with BLACK");
  tft.fillScreen(ST7735_BLACK);

}


void MainUI(){
  // main data output screen design
  // no values are displayed yet, just the text and outline boxes 
  
  tft.setTextSize(1);
  
  tft.drawRect(0,0,53,64,tft.Color565( 0x70, 0x80, 0x90));     // SPO2 rectangle outline  
  tft.setCursor(5,5);
  tft.println("SPO2");
  
  tft.drawRect(54,0,53,64,tft.Color565( 0x70, 0x80, 0x90));    // BPM rectangle outline
  tft.setCursor(59,5);
  tft.println("BPM");
  
  tft.drawRect(107,0,53,64,tft.Color565( 0x70, 0x80, 0x90));   // BP rectangle outline
  tft.setCursor(112,5);
  tft.println("BP");


  tft.drawRect(0,108,53,20,tft.Color565( 0x70, 0x80, 0x90));    // bluetooth rectangle outline
  //display bluetooth symbol from picture on sd card?
  
  tft.drawRect(107,108,53,20,tft.Color565( 0x70, 0x80, 0x90));  // battery rectangle outline
  //draw battery with small rectangles of varying colour?
  
}

/*

void UpdateDisplayValues(){

  // we don't want to constantly refresh the entire display
  // so this function will overlay black boxes over the number needing
  // to be refreshed then the new value can be displayed
  
  tft.setTextSize(2);
  
  // Updating SPO2, BPM, and BP
  tft.fillRect(x,y,width,height,ST7735_BLACK);
  tft.setCursor(x,y);
  String SPO2str = String(SPO2);
  tft.println(SPO2str);

  tft.fillRect(x,y,width,height,ST7735_BLACK);
  tft.setCursor(x,y);
  String BPMstr = String(BPM);
  tft.println(BPMstr);

  tft.fillRect(x,y,width,height,ST7735_BLACK);
  tft.setCursor(x,y);
  String sysstr = String(systolic);
  tft.println(sysstr);

  tft.fillRect(x,y,width,height,ST7735_BLACK);
  tft.setCursor(x,y);
  String diastr = String(diastolic);
  tft.println(diastr);

  // Updating Bluetooth
  if(bluetooth == false){
    tft.fillRect(x,y,width,height,ST7735_BLACK);
    tft.setCursor(x,y);
    tft.println("Disconnected");
  }
  else{
    tft.fillRect(x,y,width,height,ST7735_BLACK);
    tft.setCursor(x,y);
    tft.println("Connected");
  }


  // Updating battery percentage indicator
  String batstr = String(batteryPercent);
  tft.println(batstr);
  if(batterPercent > 75){
    // draw full battery green
  }
  else if(batteryPercent > 25){
    // draw half battery yellow
  }
  else{
    // draw empty battery red
  }


  
}
*/



void loop(){

  // generate random values for each variable
  BPM = random(50,160);                   
  SPO2 = random(80,100);                
  systolic = random(90,180);             
  diastolic = random(50,110);
  batteryPercent = random(0,100);

  //UpdateDisplayValues();
     
  delay(2000);                             
}







