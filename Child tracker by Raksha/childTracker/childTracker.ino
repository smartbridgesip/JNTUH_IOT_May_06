#include <TinyGPS++.h>    
#include <SoftwareSerial.h>
SoftwareSerial myGPRS(4,5);
TinyGPSPlus gps;                                        // Create an Instance of the TinyGPS++ object called gps
SoftwareSerial ss(2,3);
String host= "api.thingspeak.com";


String GET = "GET /update?key=9KEFWZZ302KO77MR";
String latt,lon;
char d;

void setup()  
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  myGPRS.begin(9600);
  ss.begin(9600);
  
  delay(1000);
  
  sendmyGPRS("AT");
  delay(5000);
  
  if(myGPRS.find("OK")) 
  {
      d = myGPRS.read();
      //Serial.println(d);
  }

}

void loop() // run over and over
{
  ss.listen();
  Serial.print("Latitude  : ");
  Serial.println(gps.location.lat(), 5);
  Serial.print("Longitude : ");
  Serial.println(gps.location.lng(), 4);
  smartDelay(1000); 

uploadtoThingspeak();
delay(15000);

}
void uploadtoThingspeak(){

myGPRS.listen();

sendmyGPRS("AT+CIPSHUT");
  delay(2000);
  if( myGPRS.find( "SHUT OK" ) )
  {
    
       d = myGPRS.read();
     Serial.write(d);
     //Serial.println(d);
    //return;
  }
  
sendmyGPRS("AT+CIPMUX=1");
  delay(2000);
 if( myGPRS.find( "OK" ) )
  {
       d = myGPRS.read();
      Serial.write(d);
      //Serial.println();
    //return;
  }
sendmyGPRS("AT+CGATT=1");
  delay(2000);

   if( myGPRS.find( "OK" ) )
  {
      d = myGPRS.read();
      Serial.write(d);
      //Serial.println(d);
    //return;
  }
  
String cmd = "AT+CSTT=\"bsnlnet\",\"\",\"\"";// Setup TCP connection
sendmyGPRS(cmd);
  delay(2000);

   if( myGPRS.find( "OK" ) )
  {
      d= myGPRS.read();
     Serial.write(d);
     // Serial.println();
    //return;
  }

sendmyGPRS("AT+CIICR");
  delay(2000);

   if( myGPRS.find( "OK" ) )
  {
     d = myGPRS.read();
      Serial.write(d);
      //Serial.println();
    //return;
  }

sendmyGPRS("AT+CIFSR");
  delay(2000);
   
   if( myGPRS.find( "Error" ) )
  {
    myGPRS.println( "ERROR Received" );
    //return;
  }
  else {
    d = myGPRS.read();
    Serial.write(d);
     // Serial.println();
  }
  
  cmd = "AT+CIPSTART=3,\"TCP\",\"";// Setup TCP connection
  cmd += host;
  cmd += "\",80";

sendmyGPRS(cmd);
  delay(4000);

     if( myGPRS.find("CONNECTED") )
  {
      d = myGPRS.read();
      //Serial.write(d);
      Serial.println("Connected to Thingspeak");
    //return;
  }


String cmd1 = "AT+CIPSEND=3,100";// Setup TCP connection
sendmyGPRS(cmd1);
delay(2000);

cmd = GET+ "&field1="+String(gps.location.lat(),DEC)+"&field2="+String(gps.location.lng(),DEC)+"\r\n";

if( myGPRS.find( ">" ) )
  {
    myGPRS.println(cmd);
    myGPRS.println();
    Serial.println(cmd);
  }
  else
  {
    sendmyGPRS( "AT+CIPCLOSE" );//close TCP connection
  }
  sendmyGPRS("AT+CIPCLOSE");
}


void sendmyGPRS(String cmd)
{
  myGPRS.println(cmd);
  //Serial.println(cmd);
}

static void smartDelay(unsigned long ms)                // This custom version of delay() ensures that the gps object is being "fed".
{
  unsigned long start = millis();
  do 
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}
