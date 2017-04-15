/*********************************************************************
 This is an example for our nRF51822 based Bluefruit LE modules

 Pick one up today in the adafruit shop!

 Adafruit invests time and resources providing this open source code,
 please support Adafruit and open-source hardware by purchasing
 products from Adafruit!

 MIT license, check LICENSE for more information
 All text above, and the splash screen below must be included in
 any redistribution
*********************************************************************/

#include <Arduino.h>
#include <SPI.h>
#if not defined (_VARIANT_ARDUINO_DUE_X_) && not defined (_VARIANT_ARDUINO_ZERO_)
  #include <SoftwareSerial.h>
#endif

#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"
#include "BluefruitConfig.h"

#include "MAX30100_PulseOximeter.h"


// Init Display pins

#define sclk 15
#define mosi 16
#define cs   9
#define dc   5
#define rst  0 


//TFT display libraries
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>

Adafruit_ST7735 tft = Adafruit_ST7735(cs, dc, 0);

/*=========================================================================
    APPLICATION SETTINGS

    FACTORYRESET_ENABLE       Perform a factory reset when running this sketch
   
                              Enabling this will put your Bluefruit LE module
                              in a 'known good' state and clear any config
                              data set in previous sketches or projects, so
                              running this at least once is a good idea.
   
                              When deploying your project, however, you will
                              want to disable factory reset by setting this
                              value to 0.  If you are making changes to your
                              Bluefruit LE device via AT commands, and those
                              changes aren't persisting across resets, this
                              is the reason why.  Factory reset will erase
                              the non-volatile memory where config data is
                              stored, setting it back to factory default
                              values.
       
                              Some sketches that require you to bond to a
                              central device (HID mouse, keyboard, etc.)
                              won't work at all with this feature enabled
                              since the factory reset will clear all of the
                              bonding data stored on the chip, meaning the
                              central device won't be able to reconnect.
    MINIMUM_FIRMWARE_VERSION  Minimum firmware version to have some new features
    MODE_LED_BEHAVIOUR        LED activity, valid options are
                              "DISABLE" or "MODE" or "BLEUART" or
                              "HWUART"  or "SPI"  or "MANUAL"
    -----------------------------------------------------------------------*/
    #define FACTORYRESET_ENABLE         1
    #define MINIMUM_FIRMWARE_VERSION    "0.6.6"
    #define MODE_LED_BEHAVIOUR          "MODE"
    #define REPORTING_PERIOD_MS     1000
/*=========================================================================*/

// Create the bluefruit object
/* ...hardware SPI, using SCK/MOSI/MISO hardware SPI pins and then user selected CS/IRQ/RST */
Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);

// A small helper
void error(const __FlashStringHelper*err) {
  Serial.println(err);
  while (1);
}

int32_t hrmServiceId;
int32_t hrmMeasureCharId;
int32_t hrmLocationCharId;

// PulseOximeter is the higher level interface to the sensor
// it offers:
//  * beat detection reporting
//  * heart rate calculation
//  * SpO2 (oxidation level) calculation
PulseOximeter pox;

uint32_t tsLastReport = 0;
int heart_rate = 0;
//int BP_num = 0;
//int BP_denom = 0;
int percent_O2 = 0;
int last_heart_rate = 0;
float battery_sensor = 0;

byte battery_pin = A1;

// Callback (registered below) fired when a pulse is detected
void onBeatDetected()
{
    //Serial.println("Beat!");
}

/**************************************************************************/
/*!
    @brief  Sets up the HW an the BLE module (this function is called
            automatically on startup)
*/
/**************************************************************************/
void setup(void)
{
  
  tft.initR(INITR_BLACKTAB);        // initialize a ST7735R chip, orange tab
  tft.setRotation(1);               // rotate the screen 90 degrees
  tft.fillScreen(ST7735_BLACK);     // fill screen black
  
  // Main splash screen outputed on startup
  /*
  //Serial.println("Splash Title");
  tft.setTextSize(2);
  tft.setCursor(15, 10);
  tft.setTextColor(ST7735_WHITE);
  tft.print("BioDev");
  tft.setTextColor( tft.Color565( 0xFF, 0x77, 0x00) );
  tft.print(" DEMO");
  delay(2000);

  //Serial.println("Drawing biodev logo");
  tft.drawCircle(80,80,40,tft.Color565( 0xFF, 0x77, 0x00));
  tft.drawCircle(80,80,41,tft.Color565( 0xFF, 0x77, 0x00));
  tft.drawCircle(80,80,42,tft.Color565( 0xFF, 0x77, 0x00));
  tft.drawCircle(80,80,43,tft.Color565( 0xFF, 0x77, 0x00));
  tft.drawCircle(80,80,44,tft.Color565( 0xFF, 0x77, 0x00));
  tft.fillRect(40,75,80,10, tft.Color565( 0xFF, 0x77, 0x00));
  tft.fillRect(75,40,10,80, tft.Color565( 0xFF, 0x77, 0x0));
  delay(4000);
  */


  // Clear screen
  //Serial.println("Clear screen with BLACK");
  tft.fillScreen(ST7735_BLACK);
  
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
  
  
  //while (!Serial);  // required for Flora & Micro
  delay(500);

  //Serial.begin(115200);
  //Serial.println(F("Adafruit Bluefruit Command Mode Example"));
  //Serial.println(F("---------------------------------------"));

  /* Initialise the module */
  //Serial.print(F("Initialising the Bluefruit LE module: "));

  if ( !ble.begin(VERBOSE_MODE) )
  {
    error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
  }
  //Serial.println( F("OK!") );

  if ( FACTORYRESET_ENABLE )
  {
    /* Perform a factory reset to make sure everything is in a known state */
    //Serial.println(F("Performing a factory reset: "));
    if ( ! ble.factoryReset() ){
      error(F("Couldn't factory reset"));
    }
  }

  /* Disable command echo from Bluefruit */
  ble.echo(false);

 // Serial.println("Requesting Bluefruit info:");
  /* Print Bluefruit information */
  ble.info();

  //Serial.println(F("Please use Adafruit Bluefruit LE app to connect in UART mode"));
  //Serial.println(F("Then Enter characters to send to Bluefruit"));
  //Serial.println();

  ble.verbose(false);  // debug info is a little annoying after this point!

  /* Wait for connection */
  while (! ble.isConnected()) {
      delay(500);
  }

  // LED Activity command is only supported from 0.6.6
  if ( ble.isVersionAtLeast(MINIMUM_FIRMWARE_VERSION) )
  {
    // Change Mode LED Activity
    //Serial.println(F("******************************"));
    //Serial.println(F("Change LED activity to " MODE_LED_BEHAVIOUR));
    ble.sendCommandCheckOK("AT+HWModeLED=" MODE_LED_BEHAVIOUR);
    //Serial.println(F("******************************"));
  }

  //Serial.println("Initializing MAX30100");
    // Initialize the PulseOximeter instance and register a beat-detected callback
    pox.begin();
    pox.setOnBeatDetectedCallback(onBeatDetected);

  pinMode(battery_pin, INPUT);
  
  
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
  String SPO2str = String(percent_O2);
  tft.println(SPO2str);

  tft.fillRect(60,15,40,45,ST7735_BLACK);
  String BPMstr = String(heart_rate);
  if(BPMstr.length() == 3){
    tft.setCursor(61,30);
  }else{
    tft.setCursor(69,30);
  }
  tft.println(BPMstr);

  /*
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
  */

  // Updating Bluetooth
  /*
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
  */
  

  // Updating battery percentage indicator
  /*
  tft.fillRect(108,110,48,16,ST7735_BLACK);
  tft.drawRoundRect(115,110,30,15,5,ST7735_WHITE);
  tft.drawRoundRect(145,115,3,5,1,ST7735_WHITE);
  //tft.setCursor(x,y);
  //String batstr = String(batteryPercent);
  //tft.println(batstr);
  if(battery_sensor > 75){
    // draw full battery green
    tft.fillRoundRect( 117, 111, 25, 13, 4,ST7735_GREEN  );
  }
  else if(battery_sensor > 25){
    // draw half battery yellow
    tft.fillRoundRect( 117, 111, 15, 13, 4,ST7735_YELLOW  );
  }
  else{
    // draw empty battery red
    tft.fillRoundRect( 117, 111, 5, 13, 4,ST7735_RED  );
  }
  */

  
}


/**************************************************************************/
/*!
    @brief  Constantly poll for new command or response data
*/
/**************************************************************************/
void loop(void)
{
    // Make sure to call update as fast as possible
    pox.update();

    // Check for incoming characters from Bluefruit
    ble.println("AT+BLEUARTRX");
  
    ble.readline();
    
    // Asynchronously dump heart rate and oxidation levels to the serial
    // For both, a value of 0 means "invalid"
    if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
        heart_rate = pox.getHeartRate();
        //Serial.println(heart_rate);
        
        if(abs(heart_rate - last_heart_rate)/heart_rate < 0.1){
          // update display with heart_rate
          //Serial.println("*");
          UpdateDisplayValues();
        }
        last_heart_rate = heart_rate;
        
        //BP_num = random(90, 200);
        //BP_denom = random(60, 120);
        percent_O2 = pox.getSpO2();
        
        ble.print( F("AT+BLEUARTTXF=") );
        ble.print("HR");
        ble.print(heart_rate);
        ble.println("|");
        /*
        ble.print( F("AT+BLEUARTTXF=") );
        ble.print("BP");
        ble.print(BP_num);
        ble.print("/");
        ble.print(BP_denom);
        ble.println("|");
        */

        ble.print( F("AT+BLEUARTTXF=") );
        ble.print("SpO2%");
        ble.print(percent_O2);
        ble.println("|");

        /*
        ble.print( F("AT+BLEUARTTXF=") );
        ble.print("Battery%");
        battery_sensor = analogRead(battery_pin);
        battery_sensor *= 6.6;
        battery_sensor /= 1024;
        battery_sensor = (battery_sensor/4.2)*100;
        ble.print(battery_sensor);
        ble.println("|");
        */
        
        tsLastReport = millis();

        
    }
    if (strcmp(ble.buffer, "OK") == 0) {
        // no data
    return;
    }
    // Some data was found, it's in the buffer
    //Serial.print(F("[Recv] ")); Serial.println(ble.buffer);
    ble.waitForOK();
}
