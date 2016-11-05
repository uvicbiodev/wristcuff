// Display Test
// Test several Display Adafruit_GFX functions
// for 1.8" TFT LCD Color Display (SainSmart).
// Hans Luijten, Tweaking4All.com

// Init pins
#define sclk 13
#define mosi 11
#define cs   10
#define dc   9
#define rst  8  

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>

Adafruit_ST7735 tft = Adafruit_ST7735(cs, dc, rst);

void setup() {
  // Init serial for serial monitor, so we can see debug text in the Serial Monitor Window
  Serial.begin(9600);
  Serial.println("Display Function Tests Started");

  // If your TFT's plastic wrap has a Black Tab, use the following:
  //tft.initR(INITR_BLACKTAB);   // initialize a ST7735S chip, black tab
  // If your TFT's plastic wrap has a Red Tab, use the following:
  //tft.initR(INITR_REDTAB);   // initialize a ST7735R chip, red tab
  // If your TFT's plastic wrap has a Green Tab, use the following:
  tft.initR(INITR_GREENTAB); // initialize a ST7735R chip, green tab  
  Serial.println("setup() - Init Completed");
}

void loop() {
  // This keeps repeating ...
  Serial.println("loop() - starting (again) ...");

  // Clear screen
  Serial.println("Fill screen with BLACK");
  tft.fillScreen(ST7735_BLACK);

  // Hello
  Serial.println("Hello message");
  tft.setTextSize(1);
  tft.setCursor(0, 50);
  tft.setTextColor(ST7735_WHITE);
  tft.print("   BioDEV ");
  tft.setTextColor( tft.Color565( 0xFF, 0x00, 0x00) );
  tft.print(" TEST ");
  delay(3000);

  // Clear screen
  Serial.println("Fill screen with BLACK");
  tft.fillScreen(ST7735_BLACK);


  // Draw some circles
  Serial.println("Draw some circles, with delay between open and filled cicle");
  tft.drawCircle(60,70,50,ST7735_RED);
  tft.fillRect(10,65,100,10, ST7735_RED);
  tft.fillRect(55,20,10,100, ST7735_RED);
  delay(3000);

  // Clear screen
  Serial.println("Fill screen with BLACK");
  tft.fillScreen(ST7735_BLACK);

  // Draw rectangles with rounded corners for a dialog
  // Big box
  Serial.println("Display BPM info");
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

  
   tft.setCursor(22, 38);
   tft.print("Current : 76");
   delay(50);
   tft.setCursor(22, 38);
   tft.print("Current : 73");
   delay(50);
   tft.setCursor(22, 38);
   tft.print("Current : 78");
   delay(50);
   tft.setCursor(22, 38);
   tft.print("Current : 79");
   delay(50);
   tft.setCursor(22, 38);
   tft.print("Current : 68");
   delay(50);
   tft.setCursor(22, 38);
   tft.print("Current : 89");
   delay(50);
  
  delay(6000);


  // Clear screen
  Serial.println("Fill screen with BLACK");
  tft.fillScreen(ST7735_BLACK);

}
