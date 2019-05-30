#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include<String.h>
#include <SPI.h>
#include <MFRC522.h>
#define SS_PIN D4  //D2
#define RST_PIN D3 //D1
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
int statuss = 0;
int out = 0;
void balance_func();
static int total=20000;
String volume;
int amount;
String data="";


void callback(char* topic, byte* payload, unsigned int payloadLength);

// CHANGE TO YOUR WIFI CREDENTIALS
const char* ssid = "Raghu";
const char* password = "krishna17";
int one_ltr=2;

// CHANGE TO YOUR DEVICE CREDENTIALS AS PER IN IBM BLUMIX

#define ORG "l8ekzu"
#define DEVICE_TYPE "nodemcu"
#define DEVICE_ID "9948"
#define TOKEN "9948791931"  //  Authentication Token OF THE DEVICE

//  Customise the above values --------
const char publishTopic[] = "iot-2/evt/balance/fmt/json";
char server[] = ORG ".messaging.internetofthings.ibmcloud.com";
char topic[] = "iot-2/cmd/quantity/fmt/String";// cmd  REPRESENT command type AND COMMAND IS TEST OF FORMAT STRING
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;


WiFiClient wifiClient;
PubSubClient client(server, 1883, callback, wifiClient);

int publishInterval = 5000; // 30 seconds
long lastPublishMillis;
void publishData();
void wifiConnect();
void mqttConnect();

void setup() {
  
  Serial.begin(115200);
  Serial.println();   
  wifiConnect();
  mqttConnect();
   SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();  
}

void loop() 
{

 // rfid card read logic
    if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  Serial.println();
  Serial.print(" UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
   content.toUpperCase();
  Serial.println();
  if (content.substring(1) == "30 EB 96 A3") //change UID of the card that you want to give access
  {
    Serial.println(" Access Granted ");
    Serial.println(" Welcome to smart billing system for water suppliers: ");
    delay(1000);
    
                if (millis() - lastPublishMillis > publishInterval)
                {
                  
                  publishData();
                 lastPublishMillis = millis();
                }
                
                if (!client.loop())
                {
                  mqttConnect();
                }
    //Serial.println(" AMOUNT IS DEDUCTED SUCESSFULLY ");
    Serial.println();
    //status = 1;
   
     }
  
  else   {
    Serial.println(" Access Denied ");
    delay(3000);
  }
 
}

void wifiConnect() {
  Serial.print("Connecting to "); Serial.print(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("WiFi connected, IP address: "); 
  Serial.println(WiFi.localIP());
}

void mqttConnect() {
  if (!client.connected()) 
  {
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
  
  Serial.print("callback invoked for topic: ");
  Serial.println(topic);

  for (int i = 0; i < payloadLength; i++) {
    
    volume+= (char)payload[i];
  }
  
  Serial.print("data: "+ volume);
  balance_func();
 
}

void balance_func()
{
  int a = volume.toInt();
  if(a > 1 && total > 2)
    {
     amount = a*one_ltr;
     Serial.print("price for " + String(volume) + "is :");
     Serial.println(amount);
     total = total-amount;
    }
    else 
    {
        Serial.println("amount is less");
        total=30000;
 
    }
 
    volume="";
}

void publishData() 
{

  String payload="{\"d\":{\"balance\":";
  payload += total;
  payload += "}}";
  
  Serial.print("remaining balance is");
  Serial.print(total);

  if (client.publish(publishTopic, (char*) payload.c_str())) {
    Serial.println("Publish OK");
  } else {
    Serial.println("Publish FAILED");
  }
}
