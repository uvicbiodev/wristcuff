/*
 *  Biodev
 *  Pulse Sensor Monitor
 *  Interrupt
 *  October 2016
 */


volatile int IBI_array[10] = {};                 // array to hold last ten IBI values
volatile unsigned long sampleCounter = 0;        // used to determine pulse timing
volatile unsigned long lastBeatTime = 0;         // used to find IBI
volatile int Peak = 550;                         // used to find peak in pulse wave, seeded
volatile int Trough = 500;                       // used to find trough in pulse wave, seeded
volatile int Thresh = 550;                    // used to check if meeting a threshold, seeded




void interruptSetup(){     
  // Initializes Timer2 to throw an interrupt every 2mS.
  TCCR2A = 0x02;                                // DISABLE PWM ON DIGITAL PINS 3 AND 11, AND GO INTO CTC MODE
  TCCR2B = 0x06;                                // DON'T FORCE COMPARE, 256 PRESCALER 
  OCR2A = 0X7C;                                 // SET THE TOP OF THE COUNT TO 124 FOR 500Hz SAMPLE RATE
  TIMSK2 = 0x02;                                // ENABLE INTERRUPT ON MATCH BETWEEN TIMER2 AND OCR2A
  sei();                                        // MAKE SURE GLOBAL INTERRUPTS ARE ENABLED      

  for(int i=0; i<10; i++){                    
      IBI_array[i] = 150;                             
  }

} 


// TIMER 2 INTERRUPT SERVICE ROUTINE. 
// Timer 2 takes a reading every 2 miliseconds
ISR(TIMER2_COMPA_vect){                         // triggered when Timer2 counts to 124
  cli();                                        // disable interrupts while we do this

  //digitalWrite(powerPin, HIGH);
  
  sampleCounter += 2;                           // keep track of the time in mS with this variable
  int N = sampleCounter - lastBeatTime;         // monitor the time since the last beat to avoid noise


  Signal = analogRead(pulsePin);                // read the Pulse Sensor

  
  if(N>250 && Signal < Trough){
    Trough = Signal;
  }

  if(Signal > Thresh && Signal < Peak){
    Peak = Signal;
  }

  if(N>250){

    if((Signal > Thresh) && (N>(3*IBI/5)) && (Pulse == false)){
      Pulse = true;

      IBI = sampleCounter - lastBeatTime;
      lastBeatTime = sampleCounter;

      word IBI_average = 0;                       // clear the runningTotal variable    

      for(int i=0; i<=8; i++){                    // shift data in the rate array
        IBI_array[i] = IBI_array[i+1];             
        IBI_average += IBI_array[i];                  
      }

      IBI_array[9] = IBI;                        
      IBI_average += IBI_array[9];                    
      IBI_average /= 10;                          // average the last 10 IBI values 
      BPM = 60000/IBI_average;                    // BPM is calculated from IBI_average

    }
  }

  if(Signal < Thresh && Pulse == true){

    Pulse = false;
    Thresh = (Peak-Trough)/2;
    Peak = Thresh;
    Trough = Thresh;
  
  }

  if(N > 2500){

    Thresh = 500;
    Peak = 500;
    Trough = 500;
    lastBeatTime = sampleCounter;
    
  }


  /*
  
  if((N > (IBI/5)*3) && Signal < Threshold){    // avoid dichrotic noise by waiting 3/5 of last IBI
    if (Signal < Trough){                       
      Trough = Signal;                          // keep track of lowest point in pulse wave 
    }
  }

  if(Signal > Peak && Signal > Threshold){          
    Peak = Signal;                             
  }                                        
  
  
  
  
  if ((N > (IBI/5)*3) && (Signal > Threshold)){  // avoid high frequency noise

    PulseFound = true;
    
    IBI = sampleCounter - lastBeatTime;         // measure time between beats in mS
    lastBeatTime = sampleCounter;               // keep track of time for next pulse

    word IBI_average = 0;                       // clear the runningTotal variable    

    for(int i=0; i<=8; i++){                    // shift data in the rate array
      IBI_array[i] = IBI_array[i+1];             
      IBI_average += IBI_array[i];                  
    }

    IBI_array[9] = IBI;                        
    IBI_average += IBI_array[9];                    
    IBI_average /= 10;                          // average the last 10 IBI values 
    BPM = 60000/IBI_average;                    // BPM is calculated from IBI_average
  
    Threshold = 3*Peak/5;                       // set threshold to 3/5 of the Peak

    lastBeatTime = sampleCounter;               // set lastBeatTime to current time
  }

  
  
  if(N>500){
    lastBeatTime = sampleCounter;
    Peak = 300;
    Trough = 300;
    Threshold = 300;
  }

  
  //digitalWrite(powerPin, LOW);

  */
  
  sei();                                        // enable interrupts
}// end isr





