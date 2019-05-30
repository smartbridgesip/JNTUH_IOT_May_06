#include "DHT.h"
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

#define SSD1306_LCDHEIGHT 64
#define DHTPIN D0     // what pin we're connected to
#define DHTTYPE DHT11   // DHT 11
#define OLED_ADDR   0x3C
#include <SoftwareSerial.h>

Adafruit_SSD1306 display(-1);

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif
DHT dht(DHTPIN, DHTTYPE);

int txPin = D7;
int rxPin = D6;

SoftwareSerial bt(txPin, rxPin);

void setup() {
//pinMode(D3,INPUT);   //Temperature & Humidity
pinMode(D5,INPUT);   //Air Quality
bt.begin(9600);
Serial.begin(9600);


}

void loop() {

int h = dht.readHumidity();
int t = dht.readTemperature();
int z = digitalRead(D5);
if (isnan(h) || isnan(t)) {
  Serial.println("Failed to read from DHT sensor!");
  return;
}
if (bt.available()){
    
     bt.print(t);
     bt.print("|");
     bt.print(h);
     bt.print("|");
     bt.print(z);
     bt.print("|"); 
     Serial.print(t);
     Serial.print("|");
     Serial.print(h);
     Serial.print("|");
     Serial.print(z);
     Serial.print("|");              
     delay(2000);
} 
}
