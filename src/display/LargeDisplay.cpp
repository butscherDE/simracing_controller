// LargeDisplay.cpp

#include "LargeDisplay.h"
#include <Adafruit_GFX.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

LargeDisplay::LargeDisplay(uint8_t channel) : channel(channel), display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1) {
    switchToChannel(channel); // Set the channel
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("SSD1306 allocation failed"));
        for(;;);
    } 
}

void LargeDisplay::setTitle(const String &title) {
    titleText = title;
}

void LargeDisplay::setSubtitle(const String &subtitle) {
    subtitleText = subtitle;
}

void LargeDisplay::setText(const String &text) {
    displayText = text;
}

void LargeDisplay::update() {
    switchToChannel(channel);
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(0, 0 );
    display.println(titleText);
    display.println(subtitleText);
    display.setTextSize(2, 4);
    display.setTextColor(WHITE);
    display.setCursor(0, 32);
    display.println(displayText);
    display.display(); 
}

void LargeDisplay::switchToChannel(uint8_t channel) {
  Wire.beginTransmission(0x70);  // TCA9548A address
  Wire.write(1 << channel);          // send byte to select bus
  Wire.endTransmission();
  Serial.print(channel);
}