/* 

  Battery Level Test

  -the LiPo batteries are 'maxed' at 4.2V
  -the will remain around 3.7V most of the time
  -once down to 3.2V the protection circuitry will cut them off
  -note: when no battery plugged in it will detect 4.2V

  This code assumes a voltage divider with 2 100k resistors is used

  The working range for us then is 3.2-4.2 which we can assign to a percentage of max

*/


int batPin = A1;    // select the input pin for the potentiometer

float sensorValue = 0;  // variable to store the value coming from the sensor
int batteryPercent;

void setup() {
  pinMode(batPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  // read the value from the sensor:
  sensorValue = analogRead(batPin);

  sensorValue *= 2;       // The two 100k resistor divide by 2 so multiply back   
  sensorValue *= 3.3;     // Multiply by 3.3 V, the reference voltage
  sensorValue /= 1024;    // convert to voltage

  batteryPercent = ( sensorValue / 4.2)*100;  // convert this value into a percentage of max
  
  Serial.println(batteryPercent);

  delay(1000);
  
}
