const int sensorPin = A0;         // Specify an analog pin

bool fallen = false;             // Flag for falling/rising edge detection
int lowLevel = 120;              // Low level threshold for falling edge
int highLevel = 600;            // High level threshold for rising edge
long lastTime = 0;               // Last time a rising edge was detected
long thisTime = 0;               // Current time
int thisReading;
void setup() 
{
  Serial.begin(115200);          // Highest transmission the board allows
  Serial.print("Starting tachometer...");
  pinMode(sensorPin, INPUT);
  lastTime = micros();           // Using micros for higher resolution
  
}

void loop() 
{
  thisReading = analogRead(sensorPin);    // Take a reading
 // Serial.print("Reading: ");
   //Serial.println(thisReading);
  if (thisReading > highLevel)                // check to see if above
  {                                           // the high level threshold,
    if (fallen == true)                       // if so, it was low on
    {                                         // previous pass which means a
                                              // rising edge was detected
                                              
      // Since there is only one rising edge per rotation on this
      // tachometer, and only the rising edges are being used in the RPM
      // calculation, the elapsed time since the last reading is equal to the
      // revolutions per microsecond.  To get RPM, divide (1,000,000
      // micros/second x 60 seconds/minute) by the elapsed time which
      // is equivalent to dividing 60000000 by the elasped time.
      
      thisTime = micros();
      long RPM = 60000000 / (thisTime - lastTime);
      Serial.print("   RPM: ");
      Serial.println(RPM);
      lastTime = thisTime;                      // Set the time to "last time" and
      fallen = false;                           // the edge to false so we won't count
                                                // this rise again
    }
  }
  if (thisReading < lowLevel)                   // When the edge first falls below the 
  {                                             // low level threshold, make fallen true                          
    if (fallen == false)                        
    {
      fallen = true;
    }
  }  
}
