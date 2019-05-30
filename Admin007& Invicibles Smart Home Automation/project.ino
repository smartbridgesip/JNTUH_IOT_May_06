#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
void callback(char* topic, byte* payload, unsigned int payloadLength);
const char* ssid = "harish";
const char* password = "ramadevi";
#define ORG "8690o1"
#define DEVICE_TYPE "harish"
#define DEVICE_ID "9676089118"
#define TOKEN "987654321"
String command;
char server[] = ORG ".messaging.internetofthings.ibmcloud.com";
char topic[] = "iot-2/cmd/home/fmt/String";
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;
//Serial.println(clientID);

WiFiClient wifiClient;
PubSubClient client(server, 1883, callback, wifiClient);


SoftwareSerial mySerial(D6,D7); // RX, TX 

int led1 = D3;

String state;

void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
Serial.println();
wifiConnect();
mqttConnect();
pinMode(D3, OUTPUT);
Serial.begin(115200);
mySerial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
 while (mySerial.available()){  //Check if there is an available byte to read
 delay(10); //Delay added to make thing stable 
 char c = mySerial.read(); //Conduct a serial read
 //if (c == '#') {break;} //Exit the loop when the # is detected after the word
 state += c; //Shorthand for voice = voice + c
 Serial.println(state);
  } 

// if the state is 'LED1ON' the Led1 will turn on
if (state.equals("1")) {
digitalWrite(D3, HIGH);
Serial.println("Light On");
}
// if the state is 'LED1OFF' the led1 will turn off
else  if (state.equals("2")){
digitalWrite(D3, LOW); 
Serial.println("Light Off");
}

state="";
if (!client.loop()) {
    mqttConnect();
  }
delay(100);
}

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

void callback(char* topic, byte* payload, unsigned int payloadLength) {
  Serial.print("callback invoked for topic: "); Serial.println(topic);

  for (int i = 0; i < payloadLength; i++) {
    //Serial.println((char)payload[i]);
    command += (char)payload[i];
  }
Serial.println(command);
if(command == "LIGHTON"){
  digitalWrite(D3,HIGH);
  Serial.println("Light is Switched ON");
}
else if(command == "LIGHTOFF"){
  digitalWrite(D3,LOW);
  Serial.println("Light is Switched OFF");
}
command ="";


}
