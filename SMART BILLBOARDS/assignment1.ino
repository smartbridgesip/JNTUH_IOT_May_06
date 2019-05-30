#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#define SSD1306_LCDHEIGHT 64
#define OLED_ADDR   0x3C

Adafruit_SSD1306 display(-1);

 
//-------- Customise these values -----------
const char* ssid = "pooja";
const char* password = "bunny123";

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif
#define ORG "v32l6f"
#define DEVICE_TYPE "lee_minho1"
#define DEVICE_ID "1234"
#define TOKEN "123456789"
//-------- Customise the above values --------
 
char server[] = ORG ".messaging.internetofthings.ibmcloud.com";
char topic[] = "iot-2/evt/sensordata/fmt/json";
char authMethod[] = "use-token-auth";
char token[] =TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;
 
WiFiClient wifiClient;
PubSubClient client(server, 1883,wifiClient);

void setup() {
  Serial.begin(115200);
 Serial.println();
  pinMode(D5,INPUT);
  pinMode(D8,OUTPUT);
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
  // put your main code here, to run repeatedly:
    delay(1000);
    int switch1 = digitalRead(D5);
    Serial.print(switch1);
 if(switch1==0)
  {
    digitalWrite(D8,HIGH);
    display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
    display.clearDisplay();
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(75,10);
    display.print("light is on");
    display.display();
    Serial.print("LIGHT IS ON");
    
    }
 else 
  {
    digitalWrite(D8,LOW);
    display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
    display.clearDisplay();
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(75,10);
    display.print("light is off");
    display.display();
    Serial.print("LIGHT IS OFF");
    
  }
  PublishData(switch1);
delay(1000);
}
void PublishData(int switch1){
 if (!!!client.connected()) {
 Serial.print("Reconnecting client to ");
 Serial.println(server);
 while (!!!client.connect(clientId, authMethod, token)) {
 Serial.print(".");
 delay(500);
 }
 Serial.println();
 }
  String payload = "{\"d\":{\"Switch\":";
  payload += switch1;
  payload += "}}";
 Serial.print("Sending payload: ");   
 Serial.println(payload);
  
 if (client.publish(topic, (char*) payload.c_str())) 
 {
 Serial.println("Publish ok");
 } else {
 Serial.println("Publish failed");
 }
}
