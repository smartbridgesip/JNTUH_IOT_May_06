#include <ESP8266WiFi.h>
#include <PubSubClient.h>
 
//-------- Customise these values -----------
const char* ssid = "sharanya";
const char* password = "bharathkumar";
const int trigPin = D1;  //D4
const int echoPin = D2;  //D3
//String area="Ameerpet";
// defines variables
//int duration;
//int distance;
//Serial.println(clientID);

 int flag=0;
#define ORG "b4655l"
#define DEVICE_TYPE "nodemcu"
#define DEVICE_ID "7095"
#define TOKEN "7095472760"
//-------- Customise the above values --------
 
char server[] = ORG ".messaging.internetofthings.ibmcloud.com";
char topic[] = "iot-2/evt/Data/fmt/json";
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;
 
WiFiClient wifiClient;
PubSubClient client(server, 1883,wifiClient);

void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT);
 Serial.begin(115200);
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
}
 
void loop() {
    
  int duration, distance;
  digitalWrite(trigPin, HIGH);
  delay(1000);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance =(duration/2) * 0.0343;// time * speed of sound(0.0343cm/microsec)
  Serial.print(distance);
  Serial.println(" cm");
  delay(1000);
  
  if(distance<20)
 {

   flag=0;
   Serial.print("slot1 is not empty");
   digitalWrite(D3,HIGH);
   digitalWrite(D4,LOW);
   Serial.println("\n");
   delay(1000);
 }
 else
 {
  
   flag=1;
   Serial.print("slot 1 is empty");
   digitalWrite(D3,LOW);
   digitalWrite(D4,HIGH);
   Serial.println("\n");
   delay(1000);
 }
 PublishData(flag);
 delay(1000);
 }




void PublishData(int flag){
 if (!!!client.connected()) {
 Serial.print("Reconnecting client to ");
 Serial.println(server);
 while (!!!client.connect(clientId, authMethod, token)) {
 Serial.print(".");
 delay(500);
 
 }
 Serial.println();
 }
   String payload = "{\"d\":{\"Slot1\":";
  payload += flag;
 // payload+="," "\"area\":";
  //payload += area;
  payload += "}}";
 Serial.print("Sending payload: ");
 Serial.println(payload);
 if (client.publish(topic, (char*) payload.c_str())) {
 Serial.println("Publish ok");
 } else {
 Serial.println("Publish failed");
 }
}
