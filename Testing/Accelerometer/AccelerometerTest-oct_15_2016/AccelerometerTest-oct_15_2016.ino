
const int groundpin = A0;             // analog input pin 0 -- ground
const int powerpin = 18;              // analog input pin 4 -- voltage
const int xpin = A3;                  // x-axis of the accelerometer
const int ypin = A2;                  // y-axis
const int zpin = A1;                  // z-axis (only on 3-axis models)

const int arraySize = 10;
int Xvalues[arraySize];
int Yvalues[arraySize];
int Zvalues[arraySize];
int i = 0;
int j = 0;
int k = 0;
int Xsum = 0;
int Ysum = 0;
int Zsum = 0;
bool Flag = false;


void setup(){
  Serial.begin(9600); 
}
void loop(){
  if (i< arraySize){
    Xvalues[i] = analogRead(xpin);
    Xsum = Xsum + Xvalues[i];
    i++;
    //Flag = false;
  } 
 
  if(i == arraySize){   
    if(analogRead(xpin)/(float)(Xsum/arraySize) < 0.95 || analogRead(xpin)/(float)(Xsum/arraySize) > 1.05){
     i = 0;
     Xsum = 0;
     Flag = false; 
  }else{
    Flag = true; 
  }
  }

  if (j< arraySize){
    Yvalues[j] = analogRead(ypin);
    Ysum = Ysum + Yvalues[j];
    j++;
    //Flag = false;
  }
 
  if(j == arraySize){  
    if(analogRead(ypin)/(float)(Ysum/arraySize) < 0.95 || analogRead(ypin)/(float)(Ysum/arraySize) > 1.05){
     j = 0;
     Ysum = 0;
     Flag = false; 
  }else{
    Flag =true;
  } 
  }
  
  if (k< arraySize){
    Zvalues[k] = analogRead(zpin);
    Zsum = Zsum + Zvalues[k];
    k++;
    //Flag = false;
  }
 
  if(k == arraySize){  
    if(analogRead(zpin)/(float)(Zsum/arraySize) < 0.95 || analogRead(zpin)/(float)(Zsum/arraySize) > 1.05){
     k = 0;
     Zsum = 0;
     Flag = false; 
  }else{
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
  //Serial.print(Xsum/arraySize);
  //Serial.print("\t");
  //Serial.print(Ysum/arraySize);
  //Serial.print("\t");
  //Serial.print(Zsum/arraySize);
  Serial.print(Flag);
  
  
  Serial.println();
  // delay before next reading:

    
}
