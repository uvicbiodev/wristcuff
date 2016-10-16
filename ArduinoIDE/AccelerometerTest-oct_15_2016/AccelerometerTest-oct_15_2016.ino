
const int groundpin = 18;             // analog input pin 4 -- ground
const int powerpin = 19;              // analog input pin 5 -- voltage
const int xpin = A3;                  // x-axis of the accelerometer
const int ypin = A2;                  // y-axis
const int zpin = A1;                  // z-axis (only on 3-axis models)

const int arraySize = 10;
int Xvalues[arraySize];
int Yvalues[arraySize];
int i = 0;
int Xsum = 0;
bool Flag = true;


void setup(){
  Serial.begin(9600); 
}
void loop(){
  if (i< arraySize){
    Xvalues[i] = analogRead(xpin);
    Xsum = Xsum + Xvalues[i];
    i++;
    Flag = false;
  }
 
  if(i == arraySize){  
    if(analogRead(xpin)/(float)(Xsum/arraySize) < 0.95 || analogRead(xpin)/(float)(Xsum/arraySize) > 1.05){
     i = 0;
     Xsum = 0;
     Flag = true; 
  } 
  }
  
     // print the sensor values:
  Serial.print(analogRead(xpin));
  // print a tab between values:
  Serial.print("\t");
  Serial.print(analogRead(ypin));
  // print a tab between values:
  Serial.print("\t");
  Serial.print(analogRead(zpin));
  Serial.print("\t");
  Serial.print(Xsum/arraySize);
  
  Serial.println();
  // delay before next reading:

    
}
