#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Servo.h>
#include "index_html.h"

Servo left_servo;
Servo right_servo;

// Wifi connection details
// TODO: Update to use separate .h file that's not in Version Control
const char* ssid = "Cortado";
const char* password = "Cork4Shri";

ESP8266WebServer server(80);

const int left_servo_pin = 4;
const int right_servo_pin = 5;

// TODO: Web control for fine adjust of zero trim
const int left_servo_zero = 91;
const int right_servo_zero = 94;

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

void handleSpeed(){
  String response = "";

  if(server.args() > 1) {
    // Assume arg 1 is left wheel speed (in degrees), and arg 2 is right speed
    String left_arg = server.arg(0);
    int left_speed = left_arg.toInt();

    String right_arg = server.arg(1);
    int right_speed = right_arg.toInt();

    // Left side gets negative speed since it's physically mounted the
    // other way around.
    left_servo.write(left_servo_zero - left_speed);
    right_servo.write(right_servo_zero + right_speed);

    response = "Left: ";
    response += left_arg;
    response += ", Right: ";
    response += right_arg;
  }
  else {
    response = "Not enough arguments";
  }

  server.send(200, "text/plain", response.c_str());
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
  server.on("/speed", handleSpeed);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void){
  server.handleClient();
}
