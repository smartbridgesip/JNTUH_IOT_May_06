#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Mahi's phone";
const char* password = "takeashit";

#define ORG "zt2e6a"
#define DEVICE_TYPE "naps"
#define DEVICE_ID "1811"
#define TOKEN "0987654321"
String command;
char server[] = ORG ".messaging.internetofthings.ibmcloud.com";
char topic[] = "iot-2/cmd/home/fmt/String";
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;
//Serial.println(clientID);

WiFiClient wifiClient;
PubSubClient client(server, 1883,wifiClient);
const int LDR = A0;
int input_val = 0; 
long duration, distance;
int value = analogRead(A0);

int trigPin = D8;
int echoPin = D7;
int led = D6;

void setup() {
  Serial.begin(115200);
  Serial.println();
 //LDR
  pinMode(D5,OUTPUT);
  //ultrasound 
  pinMode(led, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
   
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
    //LDR 
int value = analogRead(A0);
Serial.print(" analog sensor value is:");
Serial.println(value);

if(value>=700){
  digitalWrite(D5,HIGH);
}
else if(value<700){
  digitalWrite(D5,LOW);
}
delay(10);

//ultrasound
  digitalWrite(trigPin,LOW);
  delayMicroseconds(10);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin,LOW);
  
  duration=pulseIn(echoPin, HIGH);
  distance =duration*0.034/2;
  Serial.print("distance:");
  Serial.print(distance);
  delay(10);
  if(distance>=100){
  digitalWrite(D6,LOW);
}
else if(distance<100){
  digitalWrite(D6,HIGH);
}
delay(1000);
return;

PublishData(value,distance);
delay(1000);
}


void PublishData(char* topic,byte* payload, unsigned int payloadLength){
 if (!!!client.connected()) {
 Serial.print("Reconnecting client to ");
 Serial.println(server);
 while (!!!client.connect(clientId, authMethod, token)) {
 Serial.print(".");
 delay(500);
 }
 Serial.println();
 }
  for (int i = 0; i < payloadLength; i++) {
    //Serial.println((char)payload[i]);
    command += (char)payload[i];
  }
  Serial.println("Entered");
Serial.println(command);
input_val=analogRead(LDR);
if(command == "AUTOMATIC")
{
  Serial.println("SHIFTED TO AUTOMATIC MODE");
   if(digitalRead(D5)==HIGH && digitalRead(D6)==HIGH)
  {
    digitalWrite(D0, HIGH);
}
   else
 {
     digitalWrite(D0, LOW);
   }
}
  
else if(command == "LIGHT ON")
{
  digitalWrite(D0,HIGH);
  Serial.println("LIGHT IS TURNED ON ");
}
else if(command =="LIGHT OFF")
{
  digitalWrite(D0,LOW);
  Serial.println("LIGHT  IS TURNED OFF ");

command ="";
}
 Serial.print("Sending payload: ");
 Serial.println(payload);
  
 if (client.publish(topic, (char*) payload.c_str())) {
 Serial.println("Publish ok");
 } else {
 Serial.println("Publish failed");
 }
}














 
 
