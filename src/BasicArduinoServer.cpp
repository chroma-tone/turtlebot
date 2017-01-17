#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Servo.h>
#include "index_html.h"

Servo left_servo;
Servo right_servo;

// Jeremy's home network
const char* ssid = "Cortado";
const char* password = "Cork4Shri";

// ikeGPS WiFi
//const char* ssid = "IkeDev";
//const char* password = "onionr!ngs33";

ESP8266WebServer server(80);

const int left_servo_pin = 4;
const int right_servo_pin = 5;

const int left_servo_offset = 1;
const int right_servo_offset = 4;

void handleRoot() {
  // Null terminate index.html
  web_index_html[web_index_html_len] = '\0';
  String response((const char*)web_index_html);

  server.send(200, "text/html", response);
}

void handleNotFound(){
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void handleLed(){
  String response = "";

  if(server.args() > 0) {
    // Assume arg 1 is servo angle (in degrees)
    String arg = server.arg(0);
    int value = arg.toInt();
    left_servo.write(value + left_servo_offset);
    right_servo.write(value + right_servo_offset);

    response = "Servo pos: ";
    response += arg;

    server.send(200, "text/plain", response.c_str());
  }
}

void setup(void){
  left_servo.attach(left_servo_pin);
  right_servo.attach(right_servo_pin);

  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.on("/led", handleLed);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void){
  server.handleClient();
}
