#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7,8); // CE, CSN

const byte address[6] = "10000";

//  VARIABLES
int pulsePin = A0;                 // Pulse Sensor purple wire('S' pin)
//connected to analog pin A0
int blinkPin = 13;                // pin to blink led at each beat
int PULSE = 0;
char inbyte = 0;


// these variables are volatile because they are used during the interrupt service routine!

volatile int BPM;                   // used to hold the pulse rate
volatile int Signal;                // holds the incoming raw data
volatile int IBI = 600;             // holds the time between beats, must be seeded! 
volatile boolean Pulse = false;     // true when pulse wave is high, false when it's low
volatile boolean QS = false;        // becomes true when Arduoino finds a beat.


void setup(){
  pinMode(blinkPin,OUTPUT);         // pin that will blink to your heartbeat!
  Serial.begin(9600);               // we agree to talk fast!
  interruptSetup();                 // sets up to read Pulse Sensor signal every 2mS 
        radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}

void loop(){
 if (QS == true){                       // Quantified Self flag is true when arduino finds a heartbeat
      //Serial.print("Signal=");
      //Serial.println(Signal);           // Print pulse sensor raw data
      Serial.print("HeartBeat=");
      Serial.println(BPM);              // Print BPM (beats per Minut)
      //Serial.print("Time Interval=");
      //Serial.println(IBI);              // Print IBI (Interval between beats)
      
      PULSE = BPM;             // read the BPM to variable 
      QS = false;                       // reset the Quantified Self flag for next time    
     }
     //delay(20);
 
int val = analogRead(A1);
float mv = ( val/1024.0)*5000; 
float cel = mv/10;
float farh = (cel*9)/5 + 32;

//Serial.print("TEMPRATURE = ");
//Serial.print(cel);
//Serial.print("*C");
//Serial.println();
Serial.print("TEMPRATURE = ");
Serial.print(farh);
Serial.print("*F");
Serial.println();
delay(1000);
 int pulse=BPM;
  radio.write(&pulse, sizeof(pulse));
  delay(2000);
  
// float tem_c=cel;
// radio.write(&tem_c, sizeof(tem_c));
// delay(1000);


 float tem_f=farh;
  radio.write(&tem_f, sizeof(tem_f));
  delay(2000);
  delay(500);
}
