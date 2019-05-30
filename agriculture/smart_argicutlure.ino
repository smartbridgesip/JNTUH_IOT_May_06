#include<Servo.h>
#include "DHT.h"
#define DHTPIN D2
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
const char* ssid = "ramya";
const char* password = "ramya1245";
#define mqtt_server  "192.168.43.213"
#define DHTTYPE DHT11  
Servo servo;

float moisture;
int value1;
char moisture_value[5],temperature_value[5],humidity_value[5];
DHT dht(DHTPIN, DHTTYPE);
WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi()
{
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  } 
if(strcmp(topic,"motor")==0)
  {
    if((char)payload[0]=='1')
    {
      Serial.println("1 done");
      digitalWrite(12, HIGH); 
    } 
    else
    {
      Serial.println("0 done");
      digitalWrite(12, LOW);
    } 
  }
if(strcmp(topic,"servo")==0)
    {
    if((char)payload[0]=='1')
      {
        servo.write(90);
      } 
      else
      {
        servo.write(0);
      } 
  }

}


void reconnect()
{
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
     client.connect("clientId");// user name and password is provided by mqtt broker and repalce the topics with your one.
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.publish("hum",temperature_value);
      client.publish("temp",humidity_value);
      client.publish("mois",moisture_value);
      client.subscribe("motor");
      client.subscribe("servo");

      // ... and resubscribe
      //client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  dht.begin();
  Serial.begin(115200);
  pinMode(12,OUTPUT);
  servo.attach(2);
  servo.write(0);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
 }

void loop() {
 if (!client.connected())
  {
    reconnect();
  }  
  float t = dht.readTemperature();
float h = dht.readHumidity();

  moisture=analogRead(A0);
  value1=(moisture/10);
 Serial.print("moisture=");
 Serial.println(value1);
 Serial.print("temperature=");
 Serial.println(t);
 Serial.print("humidity=");
 Serial.println(h);
  client.loop();
  snprintf(moisture_value,5,"%d",value1);
  snprintf(temperature_value,5,"%d",t);
 snprintf(humidity_value,5,"%d",h);
  client.setCallback(callback);
  client.publish("hum",humidity_value);
  client.publish("temp",temperature_value);
  client.publish("mois",moisture_value);
  delay(1000);
 }
