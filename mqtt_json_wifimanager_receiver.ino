#include <WiFiManager.h>

#include <ArduinoJson.h>

#include "Adafruit_MQTT.h"

#include "Adafruit_MQTT_Client.h"

#include <SPIFFS.h>

#define AIO_SERVER      "io.adafruit.com"

#define AIO_SERVERPORT  1883                 

#define AIO_USERNAME    ""

#define AIO_KEY         ""

WiFiClient client;     // Create an ESP8266 WiFiClient class to connect to the MQTT server.

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);        // Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.

Adafruit_MQTT_Subscribe jsonStuff = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/jsonstuff");





void setup() {

Serial.begin(115200);

Wifimanager();

mqtt.subscribe(&jsonStuff);  

}


void loop() {

MQTT_connect();
   
Adafruit_MQTT_Subscribe *subscription;
                                                          
 while ((subscription = mqtt.readSubscription(5000))) {

  if (subscription == &jsonStuff) {

     ReceiveJson(); 
       
  }  
 }
}
  
 



void MQTT_connect() {

  int8_t ret;

  // Stop if already connected.

  if (mqtt.connected()) {

    return;

  }

 Serial.print("Connecting to MQTT... ");

uint8_t retries = 20;

  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected

       Serial.println(mqtt.connectErrorString(ret));

       Serial.println("Retrying MQTT connection in 5 seconds...");

       mqtt.disconnect();

       delay(5000);  // wait 5 seconds

       retries--;

       if (retries == 0) {

         // basically die and wait for WDT to reset me

         while (1);

       }

  }

  Serial.println("MQTT Connected!");

}

void Wifimanager(){
  
  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
    
    //WiFiManager, Local intialization. 
    WiFiManager wm;

    // reset settings - wipe stored credentials for testing. 
    //wm.resetSettings(); // Comment out after testing

    // Automatically connect using saved credentials,
    // if connection fails, it starts an access point with the specified name ( "AutoConnectAP"),
    // if empty will auto generate SSID, if password is blank it will be anonymous AP (wm.autoConnect())
    // then goes into a blocking loop awaiting configuration and will return success result

    bool res;
    
    res = wm.autoConnect("AutoConnectAP","password"); // password protected ap

    if(!res) {
        Serial.println("Failed to connect");
    } 
    else {  
        Serial.println("Connected! :)");
    }
  
  
  }

void ReceiveJson(){

  DynamicJsonDocument doc(2048);

  String json;
 
  json = (char*)jsonStuff.lastread ;

  Serial.println(json);
     
  deserializeJson(doc, json);

  bool access = doc["access"];

  int gpio = doc["gpio"];

  String sampletext = doc["sampletext"];
      
if(access == true){
   pinMode(gpio, OUTPUT); //Turn LED on specified GPIO pin
   digitalWrite(gpio, HIGH);
   delay(5000);
   digitalWrite(gpio, LOW);
   Write_Spiffs(sampletext);
   Read_Spiffs();
   }           
}

void Write_Spiffs(String Sampletext){
  if (!SPIFFS.begin(true)){
  Serial.println("Error while mounting SPIFFS");
  return;
  }
 
  File file = SPIFFS.open("/test.txt", FILE_WRITE);

if(!file){
  
  Serial.println("There was an error openning the file");
  return;
  }

if(file.print(Sampletext)){
  Serial.println("The writing was successfull");
  }
else{
  Serial.println("The writing failed");
  }  
file.close(); 
}


void Read_Spiffs(){
  if (!SPIFFS.begin(true)){
  Serial.println("Error while mounting SPIFFS");
  return;
  }
  File file2= SPIFFS.open("/test.txt", FILE_READ);

if(!file2){
  Serial.println("There was an error reading from the file");
  return;
  }
Serial.println("File content:");

while(file2.available()){

 Serial.write(file2.read()); 
}
file2.close();
   
}
  
