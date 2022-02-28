 #include <WiFiManager.h>
//#include <HTTPClient.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>

WiFiClient wifiClient;

int timeout = 30;
String deviceID;
WiFiManager manager;
WiFiManagerParameter parameter("parameterId", "Device ID", "qwerty1", 6);

//Your Domain name with URL path or IP address with path
const char* url = "https://com-4-rt.vercel.app/api/log_data.json";

float temp_random = 41.47;
float humd_random = 40.47;

String tempString = String(temp_random,1);
String humdString = String(humd_random,0);

void postHTTP(String(url), String(tempReading), String(humdReading), String(deviceName));

int x = 1;

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 3    // Digital pin connected to the DHT sensor, D2 

#define DHTTYPE    DHT11     // DHT 11
DHT dht(DHTPIN, DHTTYPE);

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

/* Uncomment the initialize the I2C address , uncomment only one, If you get a totally blank screen try the other*/
#define i2c_Address 0x3c //initialize with the I2C addr 0x3C Typically eBay OLED's
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
static const unsigned char PROGMEM logo16_glcd_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000
};


#include "OneButton.h"
//button 1
#define buttonWiFiPin 2 //D4
// The 2. parameter activeLOW is true, because external wiring sets the button to LOW when pressed.
OneButton button(buttonWiFiPin, true);
//button 2
int buttonState = 0;         // variable for reading the pushbutton status
int buttonModePin = 0;  //

int red_light_pin= 14; //D5
int green_light_pin = 12; //D6
int blue_light_pin = 13; //D7

const int buzzer = 10; //buzzer to SD3

float humi;
float tempC;
float belowHumi = 62;
float aboveHumi = 65;
float belowTempC = 29;
float aboveTempC = 30;

void setup() {
  Serial.begin(115200);

  wifiManagerSetUp();
  deviceID = parameter.getValue();
  
  pinMode (buzzer, OUTPUT) ;// set the digital IO pin mode,

  //button 2
  pinMode(buttonModePin, INPUT);
  //button 1
  button.attachClick(singleclick);
  button.attachLongPressStop(longclick);

  //RGB
  pinMode(red_light_pin, OUTPUT);
  pinMode(green_light_pin, OUTPUT);
  pinMode(blue_light_pin, OUTPUT);

  //For DHT11
  dht.begin();
  //For OLED I2C
  display.begin(i2c_Address, true); // Address 0x3C default
  //display.setContrast (0); // dim display
  display.display();
  delay(2000);
  // Clear the buffer.
  display.clearDisplay();
  // Initialize device.
  dht.begin();

}

///////////////////////////////////////////////////////////////////////////////
void loop() {
  button.tick();
  delay(100);
  humi = dht.readHumidity();
  tempC = dht.readTemperature();

  Serial.println("X:");
  Serial.println(x);

// x start from 1
  if(x==1){  //1
    manager.autoConnect("COM4RT");
    }
  else if (x>1 and x<4){  //2,3
    manager.disconnect();
    }
  else if (x>=4 and x<6){  //4,5
    manager.autoConnect("COM4RT");
    }
  else if (x>=6 and x<8){  //6,7
    manager.disconnect();
    }
  else{
    manager.autoConnect("COM4RT");
    }   
      x += 1;


  postHTTP(url, tempString,humdString, deviceID); 
  wifiConnected();



  buttonState = digitalRead(buttonModePin);
  delay(550);
  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState == HIGH) {
    digitalWrite(buzzer, LOW);
    delay(500);
    digitalWrite(buzzer, LOW);
    // Serial.println("Off");
    //    sound();
  } 
  else {
    // turn LED off:
    digitalWrite(buzzer, HIGH);
    RGB_color(255,0,0);
    delay(500);
    digitalWrite(buzzer, LOW);
    RGB_color(0,0,0);
    // Serial.println("On");
  }

  if(humi > belowHumi && humi < aboveHumi){
    if(tempC > belowTempC && tempC < aboveTempC){
      delay(1000);
      RGB_color(255,0,255); // g
    }
    else if(tempC > aboveTempC){
      delay(1000);
      RGB_color(0,255,255); // r
      }
    else if(tempC < belowTempC){
      delay(1000);
      RGB_color(255,255,0); // b
      buzzerLight();
      }
    }
  else if(humi > aboveHumi){
    if(tempC < belowTempC){
      delay(1000);
      RGB_color(255-102,255-0,255-124); // p
      }
    else {
      delay(1000);
      RGB_color(0,255,255); // r}
    }
    }
  else if(humi < belowHumi){
    if(tempC > aboveTempC){
      delay(1000);
      RGB_color(255-102,255-0,255-124); // p
      }
    else {
      delay(1000);
      RGB_color(255,255,0); // b
            buzzerLight();
    }
    }

  display.clearDisplay();
  oledDisplay(1,0,0,humi,"%");
  oledDisplay(1,30,0,tempC,"%");
  // Serial.println(humi);
  // Serial.println(tempC);

  display.display(); 
}
///////////////////////////////////////////////////////////////////////////////
void oledDisplayHeader(){
 display.setTextSize(1);
 display.setTextColor(SH110X_WHITE);
 display.setCursor(0, 0);
 display.print("Humidity");
 display.setCursor(60, 0);
 display.print("Temperature");
}

void oledDisplay(int size, int x,int y, float value, String unit){
 int charLen=12;
 int xo=x+charLen*3.2;
 int xunit=x+charLen*3.6;
 int xval = x; 
 display.setTextSize(size);
 display.setTextColor(SH110X_WHITE);
 
 if (unit=="%"){
   display.setCursor(x, y);
   display.print(value,0);
   display.print(unit);
 } else {
   if (value>99){
    xval=x;
   } else {
    xval=x+charLen;
   }
   display.setCursor(xval, y);
   display.print(value,0);
   display.drawCircle(xo, y+2, 2, SH110X_WHITE );  // print degree symbols (  )
   display.setCursor(xunit, y);
   display.print(unit);
 }
}

void RGB_color(int red_light_value, int green_light_value, int blue_light_value)
 {  
  analogWrite(red_light_pin, red_light_value);
  analogWrite(green_light_pin, green_light_value);
  analogWrite(blue_light_pin, blue_light_value);
}

void blinkLed()
{
  RGB_color(255, 0, 0);  // red
  delay(1000);
  RGB_color(0, 0, 0);  // off
  delay(1000);
}

void singleclick(){
  }

void longclick(){
  }

void buzzerLight(){
  tone(buzzer, 100); // Send 1KHz sound signal...
  delay(1000);        // ...for 1 sec
  noTone(buzzer);     // Stop sound...
  delay(1000);        // ...for 1sec
  }

void wifiManagerSetUp(){
  manager.resetSettings();
  manager.setEnableConfigPortal(true);
  manager.setCustomHeadElement("COM4RT");
  manager.setHostname("hostname");
  manager.setDebugOutput(false);  
  std::vector<const char *> menu = {"wifi","setup","sep","status","exit"};
  manager.setMenu(menu);
//      WiFiManagerParameter parameter("parameterId", "Device ID", "qwerty", 6);
  manager.addParameter(&parameter);
  manager.setClass("invert"); // dark theme
  manager.setScanDispPerc(true); // display percentages instead of graphs for RSSI
//  manager.autoConnect("COM4RT");
}

void postHTTP(String(url), String(tempReading), String(humdReading), String(deviceName)){
  unsigned long delay_datasent = 10000;  // 10sec
  HTTPClient http;
  String response;
  http.begin(wifiClient,url);
  StaticJsonDocument<200> buff;
  String jsonParams;

  http.addHeader("Content-Type", "application/json");
//  int httpResponseCode = http.POST("{\"device_id\":\"VERCEL\",\"temp\":\"33.3\",\"humidity\":\"24.25\"}");
  buff["device_id"] = deviceName;
  buff["temp"] = tempReading;
  buff["humidity"] = humdReading;
  serializeJson(buff, jsonParams);
  delay(delay_datasent);
  http.POST(jsonParams);
  response = http.getString();
  Serial.println(response);
  http.end();
}

void wifiConnected(){
    Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
}
