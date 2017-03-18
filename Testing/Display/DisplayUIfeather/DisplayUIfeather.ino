// BioDEV Display Final UI Testing
// using the feather board
// March 2017


// Init Display pins
#define sclk 15
#define mosi 16
#define cs   10
#define dc   9
#define rst  6 

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
boolean bluetooth = true;   // bluetooth boolean true=connected false=otherwise
int batteryPercent;         // battery percentage (0-100)
int waveform[160];          // used to store the waveform data

void setup(){
 
  Serial.begin(9600);      
  
  tft.initR(INITR_REDTAB);        // initialize a ST7735R chip, orange tab
  tft.setRotation(1);               // rotate the screen 90 degrees
  tft.fillScreen(ST7735_BLACK);     // fill screen black

  SplashScreen();                   // display the splashscreen
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
  //delay(2000);

  Serial.println("Drawing biodev logo");
  tft.drawCircle(80,80,40,tft.Color565( 0xFF, 0x77, 0x00));
  tft.drawCircle(80,80,41,tft.Color565( 0xFF, 0x77, 0x00));
  tft.drawCircle(80,80,42,tft.Color565( 0xFF, 0x77, 0x00));
  tft.drawCircle(80,80,43,tft.Color565( 0xFF, 0x77, 0x00));
  tft.drawCircle(80,80,44,tft.Color565( 0xFF, 0x77, 0x00));
  tft.fillRect(40,75,80,10, tft.Color565( 0xFF, 0x77, 0x00));
  tft.fillRect(75,40,10,80, tft.Color565( 0xFF, 0x77, 0x0));
  //delay(4000);


  // Clear screen
  Serial.println("Clear screen with BLACK");
  tft.fillScreen(ST7735_BLACK);

}


void MainUI(){
  // main data output screen design
  // no values are displayed yet, just the text and outline boxes 
  
  tft.setTextSize(1);
  tft.setTextColor( tft.Color565( 0xFF, 0x77, 0x00) );
  
  tft.drawRect(0,1,53,64,tft.Color565( 0x70, 0x80, 0x90));     // SPO2 rectangle outline  
  tft.setCursor(10,5);
  tft.println("SPO2");
  
  tft.drawRect(53,1,53,64,tft.Color565( 0x70, 0x80, 0x90));    // BPM rectangle outline
  tft.setCursor(70,5);
  tft.println("BPM");
  
  tft.drawRect(106,1,53,64,tft.Color565( 0x70, 0x80, 0x90));   // BP rectangle outline
  tft.setCursor(130,5);
  tft.println("BP");


  tft.drawRect(0,108,80,20,tft.Color565( 0x70, 0x80, 0x90));    // bluetooth rectangle outline
  //display bluetooth symbol from picture on sd card?
  
  tft.drawRect(106,108,53,20,tft.Color565( 0x70, 0x80, 0x90));  // battery rectangle outline
  //draw battery with small rectangles of varying colour?
  
}



void UpdateDisplayValues(){

  // we don't want to constantly refresh the entire display
  // so this function will overlay black boxes over the number needing
  // to be refreshed then the new value can be displayed
  
  tft.setTextSize(2);
  tft.setTextColor(ST7735_WHITE);
  
  // Updating SPO2, BPM, and BP
  tft.fillRect(5,15,40,45,ST7735_BLACK);
  tft.setCursor(13,30);
  String SPO2str = String(SPO2);
  tft.println(SPO2str);

  tft.fillRect(60,15,40,45,ST7735_BLACK);
  String BPMstr = String(BPM);
  if(BPMstr.length() == 3){
    tft.setCursor(61,30);
  }else{
    tft.setCursor(69,30);
  }
  tft.println(BPMstr);

  tft.fillRect(113,15,40,45,ST7735_BLACK);
  tft.setTextSize(1);
  String sysstr = String(systolic);
  String diastr = String(diastolic);
  if(sysstr.length() == 3){
    tft.setCursor(120,25);
  }else{
    tft.setCursor(125,25);
  }
  tft.println(sysstr);
  if(diastr.length() == 3){
    tft.setCursor(120,40);
  }else{
    tft.setCursor(125,40);
  }
  tft.println(diastr);
  tft.fillRect(117,35,30,2,ST7735_WHITE);


  // Updating Bluetooth
  tft.setTextSize(1);
  tft.fillRect(2,110,76,16,ST7735_BLACK);
  if(bluetooth == false){
    tft.setCursor(4,114);
    tft.println("Disconnected");
  }
  else{
    tft.setCursor(10,114);
    tft.println("Connected");
  }
  

  // Updating battery percentage indicator
  tft.fillRect(108,110,48,16,ST7735_BLACK);
  tft.drawRoundRect(115,110,30,15,5,ST7735_WHITE);
  tft.drawRoundRect(145,115,3,5,1,ST7735_WHITE);
  //tft.setCursor(x,y);
  //String batstr = String(batteryPercent);
  //tft.println(batstr);
  if(batteryPercent > 75){
    // draw full battery green
    tft.fillRoundRect( 117, 111, 25, 13, 4,ST7735_GREEN  );
  }
  else if(batteryPercent > 25){
    // draw half battery yellow
    tft.fillRoundRect( 117, 111, 15, 13, 4,ST7735_YELLOW  );
  }
  else{
    // draw empty battery red
    tft.fillRoundRect( 117, 111, 5, 13, 4,ST7735_RED  );
  }
  

  
}




void loop(){

  // generate random values for each variable
  BPM = random(50,160);                   
  SPO2 = random(80,100);                
  systolic = random(90,180);             
  diastolic = random(50,110);
  batteryPercent = random(0,100);

  if(random(0,2) == 1){
    bluetooth = true;
  }else{
    bluetooth = false;
  }

  /*
  for(int i=0;i<160;i++){
    waveform[i] = waveform[i+1];
  }
  waveform[160] = random(0,40);
  */
  
  UpdateDisplayValues();
     
  delay(2000);                             
}







