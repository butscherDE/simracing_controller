/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
*********/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "AiEsp32RotaryEncoder.h"

#include "tacho/FanTacho.h"

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const int sw = 12;
const int dt = 13;
const int clk = 14;
const int tachoPin = 27;
const int pwmPin = 26;

#define ROTARY_ENCODER_A_PIN dt
#define ROTARY_ENCODER_B_PIN clk
#define ROTARY_ENCODER_BUTTON_PIN sw
#define ROTARY_ENCODER_VCC_PIN -1
#define ROTARY_ENCODER_STEPS 4

boolean circleValues = false;

AiEsp32RotaryEncoder rotaryEncoder = AiEsp32RotaryEncoder(ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_B_PIN, ROTARY_ENCODER_BUTTON_PIN, ROTARY_ENCODER_VCC_PIN, ROTARY_ENCODER_STEPS);
FanTacho fanTacho = FanTacho(27);

void rotary_onButtonClick()
{
  static unsigned long lastTimePressed = 0; // Soft debouncing
  if (millis() - lastTimePressed < 500) {
    return;
  }
  lastTimePressed = millis();
  Serial.print("button pressed ");
  Serial.print(millis());
  Serial.println(" milliseconds after restart");
}

void displayUpdate(long percent, int rpm) {
  int val = rotaryEncoder.readEncoder();
  display.clearDisplay();
  display.setCursor(2, 20);
  display.print(val);
  display.println(" %");
  display.setCursor(2, 44);
  display.print(rpm);
  display.println(" rpm");
  display.drawRect(0, 16, 128, 24, WHITE);
  display.drawRect(0, 40, 128, 24, WHITE);
  display.display();
}

void rotary_loop() {
  //dont print anything unless value changed
  if (rotaryEncoder.encoderChanged()) {
    long rotaryEncoderState = rotaryEncoder.readEncoder();
    Serial.print("Value: ");
    Serial.println(rotaryEncoderState);
    displayUpdate(rotaryEncoderState, 1600);
  }
  if (rotaryEncoder.isEncoderButtonClicked()) {
    rotary_onButtonClick();
  }
}

void IRAM_ATTR readEncoderISR() {
  rotaryEncoder.readEncoder_ISR();
}


void rotaryEncoderSetup() {
  rotaryEncoder.begin();
  rotaryEncoder.setup(readEncoderISR);
  rotaryEncoder.setBoundaries(0, 100, circleValues);
  rotaryEncoder.setAcceleration(500);
}

void displaySetup() {
  Wire.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }
  delay(2000);
  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(4, 20);
  // display static text
  display.println("0 %");
  display.setCursor(2, 44);
  display.println("0 rpm");
  display.drawRect(0, 16, 128, 24, WHITE);
  display.drawRect(0, 40, 128, 24, WHITE);
  display.display();
}

const int pwmChannel = 0;    // PWM channel (0-15)
const int pwmFreq = 5000;    // PWM frequency in Hz (5 kHz)
const int pwmResolution = 8; // PWM resolution (1-16 bits)

void setup() {
  rotaryEncoderSetup();

  Serial.begin(115200);
  pinMode(sw, INPUT);
  pinMode(dt, INPUT);
  pinMode(clk, INPUT);

  displaySetup();
  fanTacho.initTacho();

  //pinMode(26, OUTPUT);
  ledcSetup(pwmChannel, pwmFreq, pwmResolution);
  ledcAttachPin(pwmPin, pwmChannel);
}

bool isOn = true;
const int dutyCycles[] = {0, 25, 50, 75, 100};
int currentDutyCycleIndex = 0;

long dutyCycle = 0;
void loop() {
  rotary_loop();
  fanTacho.updateTacho();
  digitalWrite(26, isOn);
  isOn = !isOn;
  

    // Set the current duty cycle
  //int dutyCycle = map(dutyCycles[currentDutyCycleIndex], 1, 100, 1, 255);
  dutyCycle = rotaryEncoder.readEncoder() * 2.55f;
  ledcWrite(pwmChannel, dutyCycle);


  // Increment the duty cycle index or reset it if it exceeds the array size

  // Delay for 5 seconds
  //delay(5000);

  delay(50); //or do whatever you need to do...
}