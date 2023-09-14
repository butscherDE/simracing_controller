// LargeDisplay.h

#ifndef LARGEDISPLAY_H
#define LARGEDISPLAY_H

#include <Arduino.h> // Include the necessary libraries here
#include <Adafruit_SSD1306.h>

class LargeDisplay {
public:
    LargeDisplay(uint8_t channel);
    void setTitle(const String &title);
    void setSubtitle(const String &subtitle);
    void setText(const String &text);
    void update();

private:
    uint8_t channel;
    Adafruit_SSD1306 display;
    String titleText;
    String subtitleText;
    String displayText;
    
    void switchToChannel(uint8_t channel);
};

#endif // LARGEDISPLAY_H
