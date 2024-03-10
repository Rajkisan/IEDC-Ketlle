#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <Fonts/FreeSansOblique10pt7b.h>
#include <NewPing.h>

#include "WiFi.h"
#include <OneWire.h>
#include <DallasTemperature.h>

// GPIO where the DS18B20 is connected to
const int oneWireBus = 25;     

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);
#define TRIGGER_PIN 5
#define ECHO_PIN 18
#define MAX_DISTANCE 200 // Maximum distance to measure, in centimeters

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

#define i2c_Address 0x3c //initialize with the I2C addr 0x3C Typically eBay OLED's
//#define i2c_Address 0x3d //initialize with the I2C addr 0x3D Typically Adafruit OLED's

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1   //   QT-PY / XIAO
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16

const unsigned char wifi [] PROGMEM = {
  // 'wifi2, 16x16px
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xe0, 0x0f, 0xf0, 0x18, 0x18, 
  0x03, 0xc0, 0x07, 0xe0, 0x04, 0x20, 0x01, 0x80, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const char* ssid = "RSSS"; //choose your wireless ssid
const char* password =  "12345678"; //put your wireless password here.

const unsigned char no_wifi [] PROGMEM = {
  // 'no_con, 16x16px
  0x00, 0x00, 0x03, 0xe0, 0x0f, 0xf8, 0x1f, 0xfc, 0x3e, 0x3e, 0x38, 0x7e, 0x70, 0xff, 0x71, 0xf7, 
  0x73, 0xc7, 0x77, 0x87, 0x3f, 0x0e, 0x3e, 0x1e, 0x1f, 0xfc, 0x0f, 0xf8, 0x03, 0xe0, 0x00, 0x00
};
const unsigned char bat_0 [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xf8, 0x7f, 0xfc, 0xff, 0xfe, 0xe0, 0x06, 0xe0, 0x07, 
  0xe0, 0x07, 0xe0, 0x06, 0xff, 0xfe, 0x7f, 0xfc, 0x3f, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
const unsigned char bat_1 [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xf8, 0x7f, 0xfc, 0xff, 0xfe, 0xec, 0x06, 0xec, 0x07, 
  0xec, 0x07, 0xec, 0x06, 0xff, 0xfe, 0x7f, 0xfc, 0x3f, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
const unsigned char bat_2 [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xf8, 0x7f, 0xfc, 0xff, 0xfe, 0xed, 0x86, 0xed, 0x87, 
  0xed, 0x87, 0xed, 0x86, 0xff, 0xfe, 0x7f, 0xfc, 0x3f, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
const unsigned char bat_3 [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xf8, 0x7f, 0xfc, 0xff, 0xfe, 0xed, 0xb6, 0xed, 0xb7, 
  0xed, 0xb7, 0xed, 0xb6, 0xff, 0xfe, 0x7f, 0xfc, 0x3f, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char temp [] PROGMEM = {
  0x01, 0xc0, 0x03, 0xe0, 0x07, 0x20, 0x07, 0xe0, 0x07, 0x20, 0x07, 0xe0, 0x07, 0x20, 0x07, 0xe0, 
  0x07, 0x20, 0x0f, 0xf0, 0x1f, 0xf8, 0x1f, 0xf8, 0x1f, 0xf8, 0x1f, 0xf8, 0x0f, 0xf0, 0x07, 0xe0
};
const unsigned char water [] PROGMEM = {
  0x00, 0x00, 0x01, 0x80, 0x03, 0xc0, 0x07, 0xe0, 0x0e, 0x70, 0x0c, 0x30, 0x1c, 0x38, 0x18, 0x18, 
  0x38, 0x1c, 0x38, 0x1c, 0x38, 0x1c, 0x1c, 0x38, 0x1f, 0xf8, 0x0f, 0xf0, 0x03, 0xc0, 0x00, 0x00
};

void setup() {
  Serial.begin(115200);
  display.begin(i2c_Address, true); // Address 0x3C default
  display.clearDisplay();
  WiFi.begin(ssid, password);
  sensors.begin();
}

void loop() { 
  display.setFont(&FreeSansOblique10pt7b);
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);

  // Voltage measurement
  float adc = analogRead(A0);
//  Serial.println("Analog=");
//  Serial.print(adc);
  float voltage = adc/254;

  Serial.println("Voltage : ");
  Serial.print(voltage);
  sensors.requestTemperatures(); 
  float temperatureC = sensors.getTempCByIndex(0);
  Serial.print(temperatureC);
  Serial.println("ºC");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    display.setCursor(0, 20);
    display.fillRect(0, 5, 16, 16, SH110X_BLACK); 
    display.println(" ");
    Serial.println("Connecting to WiFi..");
    display.drawBitmap(0, 5, no_wifi, 16, 16, 1);
    display.drawBitmap(0, 40, temp, 16, 16, 1);
    display.drawBitmap(25, 7, water, 16, 16, 1);
    display.setCursor(20, 55);
    display.fillRect(20, 40, 50, 20, SH110X_BLACK);
    display.println(temperatureC);
    display.display();
  }

  Serial.println("Connected to the WiFi network");
  Serial.println("");
//  Serial.println("WiFi connected.");
//  Serial.println("IP address: ");
//  Serial.println(WiFi.localIP());

  display.fillRect(0, 5, 16, 16, SH110X_BLACK);
  
  display.drawBitmap(0, 5, wifi, 16, 16, 1);
  display.drawBitmap(0, 40, temp, 16, 16, 1);
  display.drawBitmap(25, 7, water, 16, 16, 1);
      display.setCursor(20, 55);
      display.fillRect(20, 40, 50, 20, SH110X_BLACK);
    display.println(temperatureC);
    display.display();
  
  
  delay(2000);

  // Water Level measurement
  delay(500);
  unsigned int distance = sonar.ping_cm();
  
  
  if (distance == 0) {
    Serial.println("Error: Measurement timeout");
  } else {
    Serial.print("Distance:");
    Serial.print(distance);
    Serial.print("cm");
    Serial.println();
    if (distance > 11) {
      Serial.println("Please close lid");  
      display.setCursor(50, 20);
      display.fillRect(50, 5, 40, 20, SH110X_BLACK);
      display.println("_ _%");
      display.display();
    } else if (distance >= 9) {
      Serial.println("Empty!");
      display.fillRect(50, 5, 40, 20, SH110X_BLACK);
      display.setCursor(50, 20);
      display.println("0%");
      display.display();
    } else if (distance == 6 || distance == 7 || distance == 8) {
      Serial.println("25%");
      display.fillRect(50, 5, 40, 20, SH110X_BLACK);
      display.setCursor(50, 20);
      display.println("25%");
      display.display();
    } else if (distance == 4 || distance == 5) {
      Serial.println("50%");
      display.fillRect(50, 5, 40, 20, SH110X_BLACK);
      display.setCursor(50, 20);
      display.println("50%");
      display.display();
    } else if (distance == 3 || distance == 2) {
      Serial.println("75%");
      display.fillRect(50, 5, 40, 20, SH110X_BLACK);
      display.setCursor(50, 15);
      display.println("75%");
      display.display();
    } else if (distance == 1) {
      Serial.println("Full!");
      display.fillRect(50, 5, 40, 20, SH110X_BLACK);
      display.setCursor(50, 20);
      display.println("Full");
      display.display();
    }
  }
//  voltage=12;
    if (voltage > 16) {
      Serial.println("Full Charge");  
      display.fillRect(80, 37, 16, 20, SH110X_BLACK);
      display.drawBitmap(80, 39, bat_3, 16, 16, 1);
      display.display();
      
    }
      else if (voltage > 15.5) {
      Serial.println("Full Charge");
      display.fillRect(80, 37, 16, 20, SH110X_BLACK);
      display.drawBitmap(80, 39, bat_2, 16, 16, 1);
      display.display();  
      
    } else if (voltage >= 15) {
      Serial.println("Empty!");
      display.fillRect(80, 37, 16, 20, SH110X_BLACK);
      display.drawBitmap(80, 39, bat_1, 16, 16, 1);
      display.display();
      
    } 
      else if (voltage <= 14.7) {
      Serial.println("Empty!");
      display.fillRect(80, 37, 16, 20, SH110X_BLACK);
      display.drawBitmap(80, 39, bat_0, 16, 16, 1);
      display.display();
      
    } 
  }
