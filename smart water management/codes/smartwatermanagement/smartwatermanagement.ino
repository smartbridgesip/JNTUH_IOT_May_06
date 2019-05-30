#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>
SoftwareSerial mynodemcu(D5,D6); // rx-5, tx-6
String data;
int waterdepth;
int flowrate;

 String getValue(String data,char separator, int index)
 {
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;
 for (int i = 0; i <= maxIndex && found <= index; i++)
  {
    if (data.charAt(i) == separator || i == maxIndex){
    found++;
    strIndex[0] = strIndex[1] + 1;
    strIndex[1] = (i == maxIndex) ? i+1 : i;
  }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
 }
 

 
//-------- Customise these values -----------
const char* ssid = "manu.akhi";
const char* password = "manasa123";

#define ORG "u98tr2"
#define DEVICE_TYPE "NODEMCU"
#define DEVICE_ID "7410"
#define TOKEN "9638527410"
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
 // Starts the serial communication
mynodemcu.begin(9600);
}
 
void loop() { 
  if(mynodemcu.available())
{
 data = mynodemcu.readStringUntil('~');
}

Serial.println(data);
String a = getValue(data,'#',0);
String b = getValue(data,'#',1);
String c = getValue(data,'#',2);

Serial.print("waterdepth: ");
Serial.println(b);
Serial.print("flowrate: ");
Serial.println(c);

waterdepth = b.toInt();
flowrate = c.toInt();
PublishData(waterdepth,flowrate);
delay(1000);
}



void PublishData(int waterdepth, int flowrate){
 if (!!!client.connected()) {
 Serial.print("Reconnecting client to ");
 Serial.println(server);
 while (!!!client.connect(clientId, authMethod, token)) {
 Serial.print(".");
 delay(500);
 }
 Serial.println();
 }
  String payload = "{\"d\":{\"waterdepth\":";
  payload += waterdepth;
  payload+="," "\"flowrate\":";
  payload += flowrate;
  payload += "}}";
 Serial.print("Sending payload: ");
 Serial.println(payload);
  
 if (client.publish(topic, (char*) payload.c_str())) {
 Serial.println("Publish ok");
 } else {
 Serial.println("Publish failed");
 }
}
