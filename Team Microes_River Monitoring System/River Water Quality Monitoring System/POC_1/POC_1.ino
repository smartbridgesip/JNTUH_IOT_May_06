#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include<SoftwareSerial.h>
 
SoftwareSerial mySerial(D7, D8);

//-------- Customise these values -----------
const char* ssid = "thelonepair";
const char* password = "159374rs";
#include "DHT.h"
#define DHTPIN D2    // what pin we're connected to
#define DHTTYPE DHT11   // define type of sensor DHT 11
DHT dht (DHTPIN, DHTTYPE);
 
#define ORG "fbf9fj"
#define DEVICE_TYPE "BB-08"
#define DEVICE_ID "11222808"
#define TOKEN "08223000"
//-------- Customise the above values --------
 
char server[] = ORG ".messaging.internetofthings.ibmcloud.com";
char topic[] = "iot-2/evt/Data/fmt/json";
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;
 
WiFiClient wifiClient;
PubSubClient client(server, 1883,wifiClient);

void setup() {
  mySerial.begin(9600);
 Serial.begin(9600);
 delay(100);
 Serial.println();
 dht.begin();
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
}
 
void loop() {

float t = dht.readTemperature();
int sensorValue = analogRead(A0);// read the input on analog pin 0:
  float voltage = sensorValue * (5.0 / 1024.0); // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  //Serial.println(sensorValue);// print out the value you read:
  delay(500);
  int turb=sensorValue;
if (isnan(t))
{
Serial.println("Failed to read from DHT sensor!");
delay(1000);
return;
}
PublishData(t,turb);
delay(1000);
if(turb<700)
{
  SendMessage(turb);
}
}



void PublishData(float temp, int turb){
 if (!!!client.connected()) {
 Serial.print("Reconnecting client to ");
 Serial.println(server);
 while (!!!client.connect(clientId, authMethod, token)) {
 Serial.print(".");
 delay(500);
 }
 Serial.println();
 }
  String payload = "{\"d\":{\"temperature\":";
  payload += temp;
  payload+="," "\"Turbidity\":";
  payload += turb;
  payload += "}}";
 Serial.print("Sending payload: ");
 Serial.println(payload);
  
 if (client.publish(topic, (char*) payload.c_str())) {
 Serial.println("Publish ok");
 } else {
 Serial.println("Publish failed");
 }
}



 void SendMessage(int turb)
{
  Serial.println(".............................");
  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  mySerial.println("AT+CMGS=\"+919502867595\"\r"); // Replace x with mobile number
  delay(1000);
  mySerial.println("Water at unsafe levels : ");// The SMS text you want to send
  int unsafe=700-turb;
  mySerial.println(unsafe);
  mySerial.print("Below limits");
  delay(100);
  mySerial.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
}
