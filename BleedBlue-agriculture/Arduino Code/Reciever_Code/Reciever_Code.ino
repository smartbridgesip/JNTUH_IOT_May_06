#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define led1 D1
#define led2 D2

RF24 radio(D4,D8);                // Set up nRF24L01 radio on SPI bus plus pins 7 & 8

const byte addresses[][6] = {"00001", "00002","00003"};

struct dataStruct{
  int volt;
}sensor_data;



void setup() {

  Serial.begin(115200);
  Serial.println("starting......");
  restartRadio();                    // turn on and configure radio  
  Serial.println("restarting radio");
  radio.startListening();
  Serial.println("Listening for sensor values..."); 
  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);
  analogWrite(led1,0);
  analogWrite(led2,0);
}

void loop() {

  delay(1000);
  radio.startListening();
  if (radio.available()) 
  {
    while (radio.available()) {
      radio.read(&sensor_data, sizeof(sensor_data));
      Serial.println("Got message:");
      Serial.print("volt: ");
      Serial.println(sensor_data.volt);
      Serial.println("-----------");
      analogWrite(led1,sensor_data.volt);
      analogWrite(led2,sensor_data.volt);
//      if(sensor_data.volt>10) 
//      {
//      Serial.println(".......................lights on........");
//      analogWrite(led1,sensor_data.volt);
//      analogWrite(led2,sensor_data.volt);
//     }
//     else 
//     {
//      Serial.println(".......................lights off........");
//      analogWrite(led1,sensor_data.volt);
//      analogWrite(led2,sensor_data.volt);
//     }

    }
  
       
        radio.stopListening();
         delay(2000);
      //  sensor_data.volt=100;
        radio.write(&sensor_data, sizeof(sensor_data));

/*  restartRadio(); // turn on and configure radio
  printf("Now sending %d byte payload... ", sizeof(sensor_data));
  if ( radio.write(&sensor_data, sizeof(sensor_data) )) { // Send via radio
   
    Serial.println("TRANSMITTING TO NODE 2");
    printf("failed.\n\r");
  } 
  else {
    printf("sent.\n\r");
  }*/
        
  }
}


void restartRadio(){
  radio.begin(); // Start up the radio
  radio.setPALevel(RF24_PA_HIGH);
  radio.setDataRate(RF24_250KBPS);  
  //radio.openReadingPipe(1,pipes[1]);  
  //radio.openWritingPipe(pipes[0]);
  radio.openWritingPipe(addresses[2]); // 00003
  radio.openReadingPipe(1, addresses[1]); // 00001
}
