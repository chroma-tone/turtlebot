#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Servo.h>
#include "index_html.h"
#include "auth.h"
#include <Crypto.h>
#include <AES.h>
#include <string.h>

Servo left_servo;
Servo right_servo;

const char * ssid;
const char * password;
AES128 aes128;

// Wifi connection details
// TODO: Update to use separate .h file that's not in Version Control

ESP8266WebServer server(80);

const int left_servo_pin = 5;
const int right_servo_pin = 4;

// TODO: Web control for fine adjust of zero trim
const int left_servo_zero = 94;
const int right_servo_zero = 91;

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

    // Right side gets negative speed since it's physically mounted the
    // other way around.
    left_servo.write(left_servo_zero + left_speed);
    right_servo.write(right_servo_zero - right_speed);

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

void decryptWifiAuth(){
  Serial.println("decrypting");
  /*
  .name        = "AES-128-ECB",
    .key         = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F},
    .plaintext   = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
                    0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF},
    .ciphertext  = {0x69, 0xC4, 0xE0, 0xD8, 0x6A, 0x7B, 0x04, 0x30,
                    0xD8, 0xCD, 0xB7, 0x80, 0x70, 0xB4, 0xC5, 0x5A}

  */
  const char ciphertext1[16]  = {0x69, 0xC4, 0xE0, 0xD8, 0x6A, 0x7B, 0x04, 0x30,
                  0xD8, 0xCD, 0xB7, 0x80, 0x70, 0xB4, 0xC5, 0x5A};
  const char keytext[16] = {0x5F,0x4D,0xCC,0x3B,0x5A,0xA7,0x65,0xD6,0x1D,0x83,0x27,0xDE,0xB8,0x82,0xCF,0x99};


  Serial.println("setting key");
  aes128.setKey((uint8_t *)keytext, sizeof keytext);
  byte buffer[28];
  uint8_t * ciphertextBytes;
  Serial.println("decrypting cipher");
  aes128.decryptBlock(buffer, (uint8_t *)ciphertext);

  Serial.print("decrypted hex = ");
  for (int i =0; i < 28 ; i++){
    Serial.print( buffer[i], HEX);
    Serial.print(" ");
  }

  Serial.print(" decrypted text = ");
  char string[sizeof buffer];
  memcpy(string, buffer, sizeof buffer);
  string[sizeof buffer] = '\0';
  password = string;
  Serial.println(string);

}
void setup(void){
  left_servo.attach(left_servo_pin);
  right_servo.attach(right_servo_pin);

  Serial.begin(115200);
  decryptWifiAuth();
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
