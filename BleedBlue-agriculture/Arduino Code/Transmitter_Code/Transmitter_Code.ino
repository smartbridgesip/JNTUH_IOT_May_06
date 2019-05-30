#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//--------------------------------------------------------------------------
const byte addresses[][6] = {"00001", "00002","00003"};
String data3;
String command;
RF24 radio(D4, D8); // Set up nRF24L01 radio on SPI bus plus pins 7 & 8

void callback(char* topic, byte* payload, unsigned int payloadLength);

const char* ssid = "dhruva";
const char* password = "12345678";
//--------------------------------------------------------------------------
#define ORG "927i9m"
#define DEVICE_TYPE "esp8266nodemcu"
#define DEVICE_ID "8266"
#define TOKEN "9010070039"
//--------------------------------------------------------------------------
char server[] = ORG ".messaging.internetofthings.ibmcloud.com";
char topic[] = "iot-2/cmd/home/fmt/String";  //subscribe
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;
//Serial.println(clientID);
//---------------------------------------------------------------------------
int ldr = A0;
int val,val1;

struct dataStruct {
  int volt;
} data;
//---------------------------------------------------------------------------
WiFiClient wifiClient;
PubSubClient client(server, 1883, callback, wifiClient);
void setup() {
  Serial.begin(115200);
  Serial.println();
  pinMode(D1,OUTPUT);
  
  wifiConnect();
  mqttConnect();
  Serial.print("Sending sensor values ...\n\r");
  restartRadio();
}

//-------------------------------------------------------------------

void loop() {
  if (!client.loop()) {
    mqttConnect();
  }
//  int value = analogRead(A0);
delay(100);
}
//-------------------------------------------------------------
void wifiConnect() {
  Serial.print("Connecting to "); Serial.print(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("nWiFi connected, IP address: "); Serial.println(WiFi.localIP());
}

void mqttConnect() {
  if (!client.connected()) {
    Serial.print("Reconnecting MQTT client to "); Serial.println(server);
    while (!client.connect(clientId, authMethod, token)) {
      Serial.print(".");
      delay(500);
    }
    initManagedDevice();
    Serial.println();
  }
}
void initManagedDevice() {
  if (client.subscribe(topic)) {
    Serial.println("subscribe to cmd OK");
  } else {
    Serial.println("subscribe to cmd FAILED");
  }
}


//----------------------------------------------------------------------------------

void callback(char* topic, byte* payload, unsigned int payloadLength) {
  Serial.print("callback invoked for topic: "); Serial.println(topic);
    for (int i = 0; i < payloadLength; i++) {
    //Serial.println((char)payload[i]);
    command += (char)payload[i];
    }
    Serial.println(command);
    digitalWrite(D1,0);
    Serial.println(command);
    //-----------------------------------------------------------------------------------
   if(command=="automatic"){
      Serial.println("aut");
      delay(500);
      val1 = analogRead(ldr);
      val = map(val1,0,1023,0,255);
      Serial.print("LDR VALUE IS " );
      Serial.println(val1);
      Serial.print("AFTER MAPPING LDR VALUE IS " );
      Serial.println(val);
      if(val>0) 
      {
        Serial.println(".......................lights on........");
        data.volt = val;
        restartRadio();                    // turn on and configure radio
        printf("Now sending %d byte payload... ", sizeof(data));
        radio.write(&data , sizeof(data) );
        }
      else 
      {
        Serial.println(".......................lights off........");
        data.volt =0;
        val=0;
        restartRadio();                    // turn on and configure radio
        printf("Now sending %d byte payload... ", sizeof(data));
        radio.write(&data , sizeof(data) );
        }

        }

        
        if(command == "lightoff"){
          val=0;
          analogWrite(D1,val);
          Serial.println("Light is Switched OFF");
          restartRadio();                    // turn on and configure radio
          printf("Now sending %d byte payload... ", sizeof(data));
          radio.write(&val , sizeof(val) );
          }
        else if(command == "lighton"){
          
            val=255;
            analogWrite(D1,val);
            Serial.println("Light is Switched ON");
            restartRadio();                    // turn on and configure radio
            printf("Now sending %d byte payload... ", sizeof(data));
            radio.write(&val , sizeof(val) );
            }
command ="";
}
//---------------------------------------------------------------------------------
void restartRadio()
{

  radio.begin(); // Start up the radio
  radio.setPALevel(RF24_PA_HIGH);
  radio.setDataRate(RF24_250KBPS);
  radio.openWritingPipe(addresses[1]);
  radio.stopListening();
}
