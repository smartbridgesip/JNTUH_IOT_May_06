
// defines pins numbers
#include <SoftwareSerial.h>
SoftwareSerial myarduino(5,6); // rx-5, tx-6
const int trigPin = 9;
const int echoPin = 10;
// defines variables
long duration;
int distance;
volatile int  flow_frequency;  // Measures flow meter pulses
unsigned int  l_hour;          // Calculated litres/hour                      
unsigned char flowmeter = 3;  // Flow Meter Pin number
unsigned long currentTime;
unsigned long cloopTime;
int s;

void flow ()                  // Interruot function
{ 
   flow_frequency++;
} 

void setup() {
 
pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin, INPUT); // Sets the echoPin as an Input
Serial.begin(9600); // Starts the serial communication
myarduino.begin(9600);
pinMode(flowmeter, INPUT);
    digitalWrite(flowmeter, HIGH);
   Serial.begin(9600); 
   attachInterrupt(0, flow, RISING); // Setup Interrupt 
                                     // see http://arduino.cc/en/Reference/attachInterrupt
   sei();                            // Enable interrupts  
   currentTime = millis();
   cloopTime = currentTime;
}

void loop() {
// Clears the trigPin
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
// Reads the echoPin, returns the sound wave travel time in microseconds
duration = pulseIn(echoPin, HIGH);
// Calculating the distance
distance= duration*0.034/2;
// Prints the distance on the Serial Monitor
Serial.print("Distance: ");
Serial.println(distance);
currentTime = millis();
   // Every second, calculate and print litres/hour
   if(currentTime >= (cloopTime + 1000))
   {     
      cloopTime = currentTime;              // Updates cloopTime
      // Pulse frequency (Hz) = 7.5Q, Q is flow rate in L/min. (Results in +/- 3% range)
      l_hour = (flow_frequency * 60 / 7.5); // (Pulse frequency x 60 min) / 7.5Q = flow rate in L/hour 
      flow_frequency = 0;                   // Reset Counter

      
      Serial.print(l_hour, DEC);            // Print litres/hour
      Serial.println(" L/hour");
   }


if(myarduino.available())
{

  String data = "#" + String(distance) + "#" + String(l_hour)+ "~" ;
  myarduino.println(data);
}
}
