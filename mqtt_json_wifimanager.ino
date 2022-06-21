#include <WiFiManager.h>

#include <ArduinoJson.h>

#include "Adafruit_MQTT.h"

#include "Adafruit_MQTT_Client.h"

#define AIO_SERVER      "io.adafruit.com"

#define AIO_SERVERPORT  1883                  

#define AIO_USERNAME    ""

#define AIO_KEY         ""

WiFiClient client;  // Create an ESP8266 WiFiClient class to connect to the MQTT server.

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);        // Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.

Adafruit_MQTT_Publish jsonStuff = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/jsonstuff");




void setup() {

Serial.begin(115200);

Wifimanager();

}


void loop() {

MQTT_connect();
   
SendJson();
                                                           
delay(70000); // To not go over the Adafruit IO upload limit
 
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

         // reset device 

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

void SendJson(){
  
  DynamicJsonDocument doc(2048);

  doc["access"] = true;

  doc["gpio"]=  2;

  doc["sampletext"] = "Test for SPIFFS";

  String output = "JSON = ";

  serializeJsonPretty(doc, output);

  Serial.println(output); //To view sent Json

  char buffer[256]; // Need to put data into a buffer and then send it

  serializeJsonPretty(doc, buffer);

  jsonStuff.publish(buffer);
   
}  
