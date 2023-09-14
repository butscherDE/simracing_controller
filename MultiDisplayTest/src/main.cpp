/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/tca9548a-i2c-multiplexer-esp32-esp8266-arduino/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*********/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#include "display/LargeDisplay.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
LargeDisplay largeDisplay = LargeDisplay(0);

const char* ssid = "zentrumdermacht";
const char* password = "jesusrockt";

const char* ntpServer = "pool.ntp.org"; // NTP server to use
const long gmtOffsetSec = -25200;            // UTC offset in seconds
const int daylightOffsetSec = 3600;     // Daylight offset in seconds (1 hour)

int currentHour = 0;
int currentMinute = 0;
int currentSecond = 0;

int textSize = 3;

// Create a UDP object to handle NTP server communication
WiFiUDP ntpUDP;

// Create an NTP client to get the time
NTPClient timeClient(ntpUDP, ntpServer, gmtOffsetSec);

// Select I2C BUS
void TCA9548A(uint8_t bus){
  Wire.beginTransmission(0x70);  // TCA9548A address
  Wire.write(1 << bus);          // send byte to select bus
  Wire.endTransmission();
  Serial.print(bus);
}
 
void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Initialize and synchronize the time
  timeClient.begin();
  timeClient.update();

  // Start I2C communication with the Multiplexer
  Wire.begin();

  // Init OLED display on bus number 2
  TCA9548A(0);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  } 
  // Clear the buffer
  display.clearDisplay();

  // Init OLED display on bus number 3
  TCA9548A(1);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  } 
  // Clear the buffer
  display.clearDisplay();

  // Write to OLED on bus number 2
  TCA9548A(0);
  display.clearDisplay();
  display.setTextSize(textSize);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  // Display static text
  display.println("00");
  display.display(); 

  // Write to OLED on bus number 3
  TCA9548A(1);
  display.clearDisplay();
  display.setTextSize(textSize);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  // Display static text
  display.println("00");
  display.display(); 
}
 
void loop() {
  // Update the time client
  timeClient.update();

  // Get and store the current time
  currentHour = timeClient.getHours();
  currentMinute = timeClient.getMinutes();
  currentSecond = timeClient.getSeconds();

  // Display the current time
  Serial.print("Current Time: ");
  Serial.print(currentHour);
  Serial.print(":");
  Serial.print(currentMinute);
  Serial.print(":");
  Serial.println(currentSecond);


/*
  // Write to OLED on bus number 2
  TCA9548A(0);
  display.clearDisplay();
 // display.setTextSize(textSize);
  display.setTextSize(2);
  display.setCursor(0, 0 );
  display.println("Time");
  display.println("(GMT+7)");
  display.setTextSize(2, 4);
  display.setTextColor(WHITE);
  display.setCursor(0, 32);
  // Display static text`
  if (currentHour < 10) {
    display.print("0");
  }
  display.print(currentHour);
  display.print(":");
  if (currentMinute < 10) {
    display.print("0");
  }
  display.print(currentMinute);
  display.print(":");
  if (currentSecond < 10) {
    display.print("0");
  }
  display.println(currentSecond);
  display.display(); 
*/
  largeDisplay.setTitle("Time");
  largeDisplay.setSubtitle("(GMT-7)");
  largeDisplay.setText(String(currentSecond));


  // Write to OLED on bus number 3
  TCA9548A(1);
  display.clearDisplay();
  display.setTextSize(textSize);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  // Display static text
  display.println(currentMinute);
  display.display(); 


  // Delay for a while before updating the time again (e.g., every 1 minute)
  delay(1000);
}