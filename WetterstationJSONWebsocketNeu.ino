#include <WiFi.h>                                     // needed to connect to WiFi
#include <WebServer.h>                                // needed to create a simple webserver (make sure tools -> board is set to ESP32, otherwise you will get a "WebServer.h: No such file or directory" error)
#include <WebSocketsServer.h>                         // needed for instant communication between client and server through Websockets
#include <ArduinoJson.h>                              // needed for JSON encapsulation (send multiple variables with one string)
#include "HTML.h"  

#include <DHT.h>


#define DHTPIN 14
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);



// SSID and password of Wifi connection:
const char* ssid = "Habibi";
const char* password = "Habibi123";

// The String below "webpage" contains the complete HTML code that is sent to the client whenever someone connects to the webserver
String webpage = HTML;

// The JSON library uses static memory, so this will need to be allocated:
// -> in the video I used global variables for "doc_tx" and "doc_rx", however, I now changed this in the code to local variables instead "doc" -> Arduino documentation recomends to use local containers instead of global to prevent data corruption

// We want to periodically send values to the clients, so we need to define an "interval" and remember the last time we sent data to the client (with "previousMillis")
int interval = 1000;                                  // send data to the client every 1000ms -> 1s
unsigned long previousMillis = 0;
unsigned long previousMillis2 = 0; 

double revolutions = 0;
double windSpeed = 0;

// Initialization of webserver and websocket
WebServer server(80);                                 // the server uses port 80 (standard port for websites
WebSocketsServer webSocket = WebSocketsServer(81);    // the websocket uses port 81 (standard port for websockets

void IRAM_ATTR fonction_ISR() {
    revolutions++;  
    //ets_printf("Button pressed \n");
}

void setup() {
  Serial.begin(115200);                               // init serial port for debugging
  dht.begin();

  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(25, INPUT);
  pinMode(22, INPUT);

  WiFi.begin(ssid, password);                         // start WiFi interface
  Serial.println("Establishing connection to WiFi with SSID: " + String(ssid));     // print SSID to the serial interface for debugging
 
  while (WiFi.status() != WL_CONNECTED) {             // wait until WiFi is connected
    delay(1000);
    Serial.print(".");
  }
  Serial.print("Connected to network with IP address: ");
  Serial.println(WiFi.localIP());                     // show IP address that the ESP32 has received from router
  
  server.on("/", []() {                               // define here wat the webserver needs to do
    server.send(200, "text/html", webpage);           //    -> it needs to send out the HTML string "webpage" to the client
  });
  server.begin();                                     // start server
  
  webSocket.begin();                                  // start websocket
    
  attachInterrupt(18, fonction_ISR, FALLING);
}

void loop() {
  server.handleClient();  
  webSocket.loop(); 
unsigned long now2 = millis();
if(now2 - previousMillis2 > 60000){
  windSpeed = revolutions * 0.18;
    Serial.println(windSpeed);
    revolutions = 0;
    previousMillis2 = now2;
}

unsigned long now = millis();                       // read out the current "time" ("millis()" gives the time in ms since the Arduino started)
  if ((unsigned long)(now - previousMillis) > interval) { // check if "interval" ms has passed since last time the clients were updated
    
  String jsonString = "";                           // create a JSON string for sending data to the client
  StaticJsonDocument<200> doc;                      // create a JSON container
  JsonObject object = doc.to<JsonObject>();         // create a JSON Object


  
  //  object["temp"] = dht.readTemperature();                // write data into the JSON object -> I used "rand1" and "rand2" here, but you can use anything else
  //  object["humi"] = dht.readHumidity();
//
  //    if(!digitalRead(4)){
  //  object["himmR"] = "Nord";
  //}
//
  //if(!digitalRead(5)){
  //  object["himmR"] = "Ost";
  //}
//
  //if(!digitalRead(25)){
  //  object["himmR"] = "Sued";
  //}
//
  //if(!digitalRead(22)){
  //  object["himmR"] = "West";
  //}
  object["windS"] = windSpeed;


    serializeJson(doc, jsonString);                   // convert JSON object to string
    Serial.println(jsonString);                       // print JSON string to console for debug purposes (you can comment this out)
    webSocket.broadcastTXT(jsonString);               // send JSON string to clients
    
    previousMillis = now;                             // reset previousMillis
  }
}