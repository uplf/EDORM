/*#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

const char* ssid     = "ESP32";
const char* password = "12345678";

IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

WebServer server(80);

uint8_t led_1_pin = 4;
bool led_1_status = LOW;

uint8_t led_2_pin = 5;
bool led_2_status = LOW;


String send_html(uint8_t led_1_stat, uint8_t led_2_stat)
{
    String ptr = "<!DOCTYPE html> <html>\n";
    ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
    ptr += "<title>LED Control</title>\n";
    ptr += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
    ptr += "body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
    ptr += ".button {display: block;width: 80px;background-color: #3498db;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
    ptr += ".button-on {background-color: #3498db;}\n";
    ptr += ".button-on:active {background-color: #2980b9;}\n";
    ptr += ".button-off {background-color: #34495e;}\n";
    ptr += ".button-off:active {background-color: #2c3e50;}\n";
    ptr += "p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
    ptr += "</style>\n";
    ptr += "</head>\n";
    ptr += "<body>\n";
    ptr += "<h1>ESP32 Web Server</h1>\n";
    ptr += "<h3>Using Access Point(AP) Mode</h3>\n";

    if (led_1_stat)
        ptr += "<p>LED1 Status: ON</p><a class=\"button button-off\" href=\"/LED1Off\">OFF</a>\n";
    else
        ptr += "<p>LED1 Status: OFF</p><a class=\"button button-on\" href=\"/LED1On\">ON</a>\n";

    if (led_2_stat)
        ptr += "<p>LED2 Status: ON</p><a class=\"button button-off\" href=\"/LED2Off\">OFF</a>\n";
    else
        ptr += "<p>LED2 Status: OFF</p><a class=\"button button-on\" href=\"/LED2On\">ON</a>\n";

    ptr += "<style>\n";
    ptr += "body {margin-top: 15%;}\n";
    ptr += ".footer-text { font-size: 30px; }";
    ptr += ".bold-text { font-weight: bold; }";
    ptr += "</style>\n";

    ptr += "<div class='footer'>";
    ptr += "<p class='footer-text'>This software was written by <span class='bold-text'>Grayson Zheng</span>.</p>";
    ptr += "</div>\n";

    ptr += "</body>\n";
    ptr += "</html>\n";
    return ptr;
}


void handle_on_connect()
{
    led_1_status = LOW;
    led_2_status = LOW;
    Serial.println("GPIO4 Status: OFF | GPIO5 Status: OFF");
    server.send(200, "text/html", send_html(led_1_status, led_2_status)); 
}

void handle_led_1_turn_on()
{
    led_1_status = HIGH;
    Serial.println("GPIO4 Status: ON");
    server.send(200, "text/html", send_html(true, led_2_status)); 
}

void handle_led_1_turn_off()
{
    led_1_status = LOW;
    Serial.println("GPIO4 Status: OFF");
    server.send(200, "text/html", send_html(false, led_2_status)); 
}

void handle_led_2_turn_on()
{
    led_2_status = HIGH;
    Serial.println("D5 Status: ON");
    server.send(200, "text/html", send_html(led_1_status, true)); 
}

void handle_led_2_turn_off()
{
    led_2_status = LOW;
    Serial.println("GPIO5 Status: OFF");
    server.send(200, "text/html", send_html(led_1_status, false)); 
}

void handle_not_found()
{
    server.send(404, "text/plain", "Not found");
}



void setup()
{
    Serial.begin(115200);
    pinMode(led_1_pin, OUTPUT);
    pinMode(led_2_pin, OUTPUT);

    WiFi.softAP(ssid, password);
    WiFi.softAPConfig(local_ip, gateway, subnet);
    delay(100);

    server.on("/", handle_on_connect);
    server.on("/LED1On", handle_led_1_turn_on);
    server.on("/LED1Off", handle_led_1_turn_off);
    server.on("/LED2On", handle_led_2_turn_on);
    server.on("/LED2Off", handle_led_2_turn_off);
    server.onNotFound(handle_not_found);

    server.begin();
    Serial.println("HTTP server started");
}



void loop()
{
    server.handleClient();
    if (led_1_status) 
        digitalWrite(led_1_pin, HIGH);
    else
    	digitalWrite(led_1_pin, LOW);

    if (led_2_status)
        digitalWrite(led_2_pin, HIGH);
    else
    	digitalWrite(led_2_pin, LOW);
}
*/

/*
  Listfiles

  This example shows how to print out the files in a
  directory on a SD card. Pin numbers reflect the default
  SPI pins for Uno and Nano models

  The circuit:
   SD card attached to SPI bus as follows:
 ** SDO - pin 11
 ** SDI - pin 12
 ** CLK - pin 13
 ** CS - depends on your SD card shield or module.
 		Pin 10 used here for consistency with other Arduino examples
    (for MKR Zero SD: SDCARD_SS_PIN)

  created   Nov 2010
  by David A. Mellis
  modified 9 Apr 2012
  by Tom Igoe
  modified 2 Feb 2014
  by Scott Fitzgerald
  modified 24 July 2020
  by Tom Igoe
  
  This example code is in the public domain.

*/

#include <Arduino.h>
    int i=1;
void setup(){

}
void loop(){
    i++;
}