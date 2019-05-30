#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(D4,D8); // CE, CSN
const byte address[6] = "10000";
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
 
//-------- Customise these values -----------
const char* ssid = "OPPO A3s";
const char* password = "nikhila@2000";
#define ORG "oa4vcj"
#define DEVICE_TYPE "nodemcu2000"
#define DEVICE_ID "2000"
#define TOKEN "123456789"
//-------- Customise the above values --------
 
char server[] = ORG ".messaging.internetofthings.ibmcloud.com";
char topic[] = "iot-2/evt/sensordata/fmt/json";
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;
 
WiFiClient wifiClient;
PubSubClient client(server, 1883,wifiClient);

void setup() {
 Serial.begin(9600);
 Serial.println();
 Serial.print("Connecting to ");
 Serial.print(ssid);
 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) {
 delay(500);
 Serial.print(".");
 } 
 Serial.println("");
 
 Serial.print("WiFi connected, IP address: ");
 Serial.println(WiFi.localIP());
 
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
  
}
void loop() {
  if (radio.available()) {
  
    int pulse;
    radio.read(&pulse, sizeof(pulse));
    Serial.print("pulse:");
    Serial.println(pulse);
    delay(2000);
    {
    if(pulse > 200)
    {
      return;
      }
    }
    /*float temp_c;
    radio.read(&temp_c, sizeof(temp_c));
    Serial.print("temp_c:");
    Serial.println(temp_c);
    delay(1000);*/
    
    float temp_f;
    radio.read(&temp_f, sizeof(temp_f));
    Serial.print("temp_f:");
    Serial.println(temp_f);
    delay(1000);
  
    PublishData(pulse,temp_f);// USER DEFINED FUNCTION
    
  }
}

void PublishData( int pulse, float temp_f){
 if (!!!client.connected()) {
 Serial.print("Reconnecting client to ");
 //Serial.println(server);
 while (!!!client.connect(clientId, authMethod, token)) {
 Serial.print(".");
 delay(500);
 }
 Serial.println();
 }

  String payload = "{\"d\":{\"pulse\":"; 
  payload += pulse;
  payload+="," "\"temp_f\":";
  payload += temp_f;
  payload += "}}";
 Serial.print("Sending payload: ");
 Serial.println(payload);
  
 if (client.publish(topic, (char*) payload.c_str())) //conversion of string to char
 {
 Serial.println("Publish ok");
 } else {
 Serial.println("Publish failed");
 }
}
