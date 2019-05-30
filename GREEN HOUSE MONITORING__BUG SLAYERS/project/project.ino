#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Servo.h>
Servo myservo7;
Servo myservo8;
int pos;
void callback(char* topic, byte* payload, unsigned int payloadLength);
 
//-------- Customise these values -----------
const char* ssid = "trinathsai";
const char* password = "12345678";
#include "DHT.h"
#define DHTPIN D2    // what pin we're connected to
#define DHTTYPE DHT11   // define type of sensor DHT 11
DHT dht (DHTPIN, DHTTYPE);

//device credentials of watson iot platform to connect nodemcu
#define ORG "0xsri6"
#define DEVICE_TYPE "mcu"
#define DEVICE_ID "1948"
#define TOKEN "123456789"
//-------- Customise the above values --------
 
char server[] = ORG ".messaging.internetofthings.ibmcloud.com";
char topice[] = "iot-2/evt/sensordata/fmt/json";//(evt-event,fmt-format)
char topicc[] = "iot-2/cmd/control/fmt/String";//subscribe
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;



 
WiFiClient wifiClient;
PubSubClient client(server, 1883,callback,wifiClient);//(MQTT port no:1883)

void setup() {
 Serial.begin(115200);
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

  pinMode(A0,INPUT);//LDR & MOISTURE
  pinMode(D1,OUTPUT);//LED
  pinMode(D7,OUTPUT);//DC MOTOR
  myservo8.attach(D8);
  mqttConnect();

}
 
void loop() {
   if (!client.loop()) {
    mqttConnect();
  }
delay(100);

float h = dht.readHumidity();
float t = dht.readTemperature();
int ldr;

if (isnan(h) || isnan(t))
{
Serial.println("Failed to read from DHT sensor!");
delay(500);
//return;
}
float mp;
  
  mp = ( 100.00 - ( (analogRead(A0)/1023.00) * 100.00 ) );

  Serial.print("Soil Moisture(in Percentage) = ");
  Serial.print(mp);
  Serial.println("%");
  delay(1000);
  int val=analogRead(A0);
 Serial.print("analog sensor value is:");
 Serial.println(val);
 delay(500);
 
 if(val<1000)
 {
  digitalWrite(D0,LOW);
  ldr=0;
 }
 else{
   digitalWrite(D0,HIGH);
   ldr=1;}

  delay(400);

void PublishData(float temp, float humid, int ldr, float mp);

PublishData(t,h,ldr,mp);
Serial.println(ldr);
delay(300);
}
void mqttConnect() {
  if (!client.connected()) {
    Serial.print("Reconnecting MQTT client to "); Serial.println(server);
    while (!client.connect(clientId, authMethod, token)) {
      Serial.print(".");
      delay(300);
    }
    initManagedDevice();
    Serial.println();
  }
}

void initManagedDevice() {
  if (client.subscribe(topicc)) {
    Serial.println("subscribe to cmd OK");
  } else {
    Serial.println("subscribe to cmd FAILED");
  }
}

void callback(char* topicc, byte* payload, unsigned int payloadLength) 
{

  String command;
  Serial.print("callback invoked for topicc: ");
  Serial.println(topicc);

  for (int i = 0; i < payloadLength; i++) {
    //Serial.println((char)payload[i]);
    command += (char)payload[i];
  }
Serial.println(command);
if(command == "lighton"){
  digitalWrite(D1,HIGH);//lighton
  Serial.println("Light is Switched ON");
}
else if(command == "lightoff"){
  digitalWrite(D1,LOW);
  Serial.println("Light is Switched OFF");
}
else if(command == "fanon"){
  digitalWrite (D7, HIGH);
  Serial.println("Exhaustfan is Switched ON");
}
else if(command == "fanoff"){
  digitalWrite (D7,LOW);
  Serial.println("Exhaustfan is Switched OFF");
}
  
else if(command == "tapon"){
   for (pos = 0; pos <= 360; pos += 1) { 
    myservo8.write(pos);              
    delay(1); }
  Serial.println("Tap is Switched ON");
}
else if(command == "tapoff"){
  for (pos = 360; pos >= 0; pos -= 1) { 
    myservo8.write(pos);             
    delay(1);                      
  }
  Serial.println("Tap is Switched OFF");
}
command ="";
}




void PublishData(float temp, float humid, int ldr, float mp){
 if (!!!client.connected()) {
 Serial.print("Reconnecting client to ");
 Serial.println(server);
 while (!!!client.connect(clientId, authMethod, token)) {
 Serial.print(".");
 delay(500);
 }
 Serial.println();
 }
 //syntax for json format
  String payload = "{\"d\":{\"temperature\":";
  payload += temp;
  payload+="," "\"humidity\":";
  payload += humid;
  payload+="," "\"lvalue\":";
  payload += ldr;
  payload+="," "\"moisturepercentage\":";
  payload += mp;
  payload += "}}";
  
 Serial.print("Sending payload: ");
 Serial.println(payload);
  
 if (client.publish(topice, (char*) payload.c_str())) {
 Serial.println("Publish ok");
 } else {
 Serial.println("Publish failed");
 }
}
 
