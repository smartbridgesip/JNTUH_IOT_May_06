#include <Wire.h>
#include <RtcDS3231.h>
#include <ESP8266WiFi.h>
//#include <ESP8266WebServer.h>
#include <PubSubClient.h>
RtcDS3231<TwoWire> Rtc(Wire);
//ESP8266WebServer server(80);

const char* ssid = "moto e5 plus 1371";
const char* password =  "12345678";
#define ORG "56fc1a"
#define DEVICE_TYPE "newdevice"
#define DEVICE_ID "new"
#define TOKEN "123456789"
//-------- Customise the above values --------
 
char server[] = ORG ".messaging.internetofthings.ibmcloud.com";
char topic[] = "iot-2/evt/sensorData/fmt/json";
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;
 
WiFiClient wifiClient;
PubSubClient client(server, 1883,wifiClient);

int hh=0,mm=0;

void setup () 
{
  int frequency=1000; //Specified in Hz
  int buzzPin=D5; 
  int timeOn=1000; //specified in milliseconds
  int timeOff=1000; //specified in millisecods
    Serial.begin(115200);
    Rtc.Begin();

    RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
    if (!Rtc.IsDateTimeValid()) 
    {
        Serial.println("RTC lost confidence in the DateTime!");
        Rtc.SetDateTime(compiled);
    }

    RtcDateTime now = Rtc.GetDateTime();
    
    Rtc.Enable32kHzPin(false);
    Rtc.SetSquareWavePin(DS3231SquareWavePin_ModeNone);
    Serial.print("Connecting to ");
    Serial.print(ssid);
    WiFi.begin(ssid, password);  //Connect to the WiFi network
 
    while (WiFi.status() != WL_CONNECTED) {  //Wait for connection
      delay(500);
      Serial.println("Waiting to connect...");
    }
     Serial.println("");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());  //Print the local IP
  
    //server.on("/", handle_index);
    
    //server.begin(); //Start the server
}

void loop () 
{
    //server.handleClient(); //Handling of incoming requests
    RtcDateTime now = Rtc.GetDateTime();
    //Print RTC time to Serial Monitor
    noTone(D5);

    delay(3000);
  
    tone(D5,4000);
    
    Serial.print("Date:");
    Serial.println(now.Day(), DEC);
    int a = now.Year();
    Serial.print(a);
    Serial.print('/');
    Serial.print(now.Month(), DEC);
    int b = now.Month();
    Serial.print(b);
    Serial.print('/');
    Serial.print(now.Year(), DEC);
    int c = now.Day();
    Serial.print(c);
    Serial.print(" Time:");
    Serial.print(now.Hour(), DEC);
    int d = now.Hour();
    Serial.print(d);
    Serial.print(':');
    Serial.print(now.Minute(), DEC);
    int e = now.Minute();
    Serial.print(e);
    Serial.print(':');
    Serial.print(now.Second(), DEC);
     int f = now.Second();
     Serial.print(f);
    delay(1000); // one second
    PublishData(a,b,c,d,e,f);
    delay(1000);
}

//void handle_index() {
 // server.send(200, "text/plain", "This is an index page.");
//}
void PublishData(int a, int b, int c, int d, int e, int f){
 if (!!!client.connected()) {
 Serial.print("Reconnecting client to ");
 Serial.println(server);
 while (!!!client.connect(clientId, authMethod, token)) {
 Serial.print(".");
 delay(500);
 }
 Serial.println();
 }
  String payload = "{\"d\":{\"Date\":";
  payload += a;
  payload +="/";
  payload += b;
  payload +="/";
  payload += c;
  payload +="," "\"Time\":";
  payload += d;
  payload +=":";
  payload += e;
  payload +=":";
  payload += f;
  payload += "}}";
 Serial.print("Sending payload: ");
 Serial.println(payload);
  
 if (client.publish(topic, (char*) payload.c_str())) {
 Serial.println("Publish ok");
 } else {
 Serial.println("Publish failed");
 }
}
