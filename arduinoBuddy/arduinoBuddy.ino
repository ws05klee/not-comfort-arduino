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

#define i2c_Address 0x3c //initialize with the I2C addr 0x3C Typically eBay OLED's

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1   //   QT-PY / XIAO
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

// #define LOGO16_GLCD_HEIGHT 16
// #define LOGO16_GLCD_WIDTH  16
// static const unsigned char PROGMEM logo16_glcd_bmp[] =
// { B00000000, B11000000,
//   B00000001, B11000000,
//   B00000001, B11000000,
//   B00000011, B11100000,
//   B11110011, B11100000,
//   B11111110, B11111000,
//   B01111110, B11111111,
//   B00110011, B10011111,
//   B00011111, B11111100,
//   B00001101, B01110000,
//   B00011011, B10100000,
//   B00111111, B11100000,
//   B00111111, B11110000,
//   B01111100, B11110000,
//   B01110000, B01110000,
//   B00000000, B00110000
// };

#include <ESP8266WiFi.h>
#include <WiFiManager.h>
WiFiManager manager;
WiFiManagerParameter parameter("parameterId", "Device ID", "ytrewq", 6);


#define buttonWiFiPin 0 //D3 //2 - D4
int buttonPressStatus = 0;


//////////////////////////post http///////////////////////////
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>

WiFiClientSecure wifiClient;
int timeout = 30;
String deviceID;
const char* url = "https://com-4-rt.vercel.app/api/log_data.json";

// float temp_random = 41.47;
// float humd_random = 40.47;

void postHTTP(String(url), String(tempReading), String(humdReading), String(deviceName));

int x = 0;

/////////////////////RGB
int red_light_pin= 14; //D5
int green_light_pin = 12; //D6
int blue_light_pin = 13; //D7
/////////////////////RGB

////////////////buzzer//////////
const int buzzer = 10; //buzzer to SD3
int buttonState = 0;         // variable for reading the pushbutton status
int buttonPin = 2; //0

float humi;
float tempC;
float belowHumi = 62;
float aboveHumi = 65;
float belowTempC = 29.10;
float aboveTempC = 29.20;

// 'humi', 24x24px
const unsigned char humiBitMap [] PROGMEM = {
	0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x66, 0x00, 0x00, 0xc3, 0x00, 0x00, 
	0xc1, 0x00, 0x01, 0x81, 0x80, 0x03, 0x00, 0xc0, 0x06, 0x00, 0x60, 0x06, 0x60, 0x60, 0x0c, 0xf0, 
	0x30, 0x08, 0xb1, 0x10, 0x18, 0xe3, 0x18, 0x18, 0x06, 0x18, 0x10, 0x0c, 0x08, 0x10, 0x18, 0x08, 
	0x18, 0x30, 0x18, 0x18, 0x67, 0x18, 0x08, 0xcd, 0x10, 0x0c, 0x0f, 0x30, 0x06, 0x07, 0x60, 0x03, 
	0x00, 0xc0, 0x01, 0xe7, 0x80, 0x00, 0x7e, 0x00
};

// 'temp1', 24x24px
const unsigned char tempBitMap [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0xc8, 0x00, 0x00, 0x89, 0xe0, 0x00, 0x88, 0x00, 0x00, 
	0x88, 0x00, 0x00, 0x89, 0xe0, 0x00, 0x88, 0x00, 0x00, 0x88, 0x00, 0x00, 0xa9, 0xe0, 0x00, 0xa8, 
	0x00, 0x00, 0xa8, 0xc0, 0x00, 0xa8, 0xc0, 0x01, 0xac, 0x00, 0x03, 0x72, 0x00, 0x02, 0xfb, 0x00, 
	0x05, 0xfd, 0x00, 0x05, 0xfd, 0x00, 0x05, 0xfd, 0x00, 0x06, 0xfd, 0x00, 0x02, 0x72, 0x00, 0x01, 
	0x86, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x00, 0x00
};

// 'comfort', 40x40px
const unsigned char comfortBitMap [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0x80, 0x00, 0x00, 0x0f, 0xff, 0xf0, 0x00, 0x00, 
	0x3f, 0x81, 0xfc, 0x00, 0x00, 0x7c, 0x00, 0x3e, 0x00, 0x01, 0xf0, 0x00, 0x0f, 0x80, 0x01, 0xc0, 
	0x00, 0x03, 0x80, 0x07, 0x80, 0x00, 0x01, 0xe0, 0x07, 0x00, 0x00, 0x00, 0xe0, 0x0e, 0x00, 0x00, 
	0x00, 0x70, 0x1c, 0x00, 0x00, 0x00, 0x38, 0x1f, 0xfe, 0x00, 0x7f, 0xf8, 0x3f, 0xff, 0xc3, 0xff, 
	0xfc, 0x3f, 0xff, 0xff, 0xff, 0xfc, 0x3f, 0xff, 0xff, 0xff, 0xfc, 0x7f, 0xff, 0xe7, 0xff, 0xfe, 
	0x7f, 0xff, 0xe7, 0xff, 0xfe, 0x6f, 0xff, 0xe7, 0xff, 0xf6, 0x6f, 0xff, 0xc3, 0xff, 0xf6, 0x67, 
	0xff, 0xc3, 0xff, 0xe6, 0x67, 0xff, 0x81, 0xff, 0xe6, 0x63, 0xff, 0x80, 0xff, 0xc6, 0x60, 0xfe, 
	0x00, 0x7f, 0x06, 0x70, 0x00, 0x00, 0x00, 0x0e, 0x70, 0x00, 0x00, 0x00, 0x0e, 0x30, 0x00, 0x00, 
	0x00, 0x0c, 0x38, 0x18, 0x00, 0x18, 0x1c, 0x38, 0x1e, 0x00, 0x78, 0x1c, 0x1c, 0x0f, 0xc3, 0xf0, 
	0x38, 0x1c, 0x0f, 0xff, 0xf0, 0x38, 0x0e, 0x03, 0xff, 0xc0, 0x70, 0x07, 0x01, 0xff, 0x80, 0xf0, 
	0x07, 0x80, 0x3c, 0x01, 0xe0, 0x01, 0xc0, 0x00, 0x03, 0xc0, 0x01, 0xf0, 0x00, 0x0f, 0x80, 0x00, 
	0x7c, 0x00, 0x3e, 0x00, 0x00, 0x3f, 0x81, 0xfc, 0x00, 0x00, 0x0f, 0xff, 0xf0, 0x00, 0x00, 0x01, 
	0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// 'abnormal', 40x40px
const unsigned char uncomfortBitMap [] PROGMEM = {
	0x00, 0x03, 0xff, 0xc0, 0x00, 0x00, 0x0f, 0xff, 0xf0, 0x00, 0x00, 0x3c, 0x00, 0x3c, 0x00, 0x00, 
	0xf0, 0x00, 0x0f, 0x00, 0x01, 0xc0, 0x00, 0x03, 0x80, 0x03, 0x80, 0x00, 0x01, 0xc0, 0x07, 0x00, 
	0x00, 0x00, 0xe0, 0x0e, 0x00, 0x81, 0x00, 0x70, 0x1c, 0x00, 0xc3, 0x00, 0x38, 0x18, 0x01, 0x81, 
	0x80, 0x18, 0x30, 0x07, 0x81, 0xe0, 0x0c, 0x30, 0x67, 0x00, 0xe6, 0x0c, 0x60, 0x60, 0x00, 0x06, 
	0x06, 0x60, 0x30, 0x00, 0x0c, 0x06, 0xc0, 0x3c, 0x00, 0x3c, 0x03, 0xc0, 0x0f, 0x81, 0xf0, 0x03, 
	0xc4, 0x7f, 0x81, 0xfe, 0x03, 0xce, 0x3e, 0x00, 0x7c, 0x03, 0xce, 0x00, 0x00, 0x00, 0x03, 0xdf, 
	0x00, 0x00, 0x00, 0x03, 0xdb, 0x00, 0x00, 0x00, 0x03, 0xf3, 0x00, 0x00, 0x00, 0x03, 0xdb, 0x00, 
	0x00, 0x00, 0x03, 0xdf, 0x00, 0x00, 0x00, 0x03, 0xce, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 
	0x00, 0x03, 0x60, 0x07, 0x6f, 0x60, 0x06, 0x60, 0x1f, 0xff, 0xf8, 0x06, 0x30, 0x19, 0x99, 0x98, 
	0x0c, 0x30, 0x00, 0x00, 0x00, 0x0c, 0x18, 0x00, 0x00, 0x00, 0x18, 0x1c, 0x00, 0x00, 0x00, 0x38, 
	0x0e, 0x00, 0x00, 0x00, 0x70, 0x07, 0x00, 0x00, 0x00, 0xe0, 0x03, 0x80, 0x00, 0x01, 0xc0, 0x01, 
	0xc0, 0x00, 0x03, 0x80, 0x00, 0xf0, 0x00, 0x0f, 0x00, 0x00, 0x3c, 0x00, 0x3c, 0x00, 0x00, 0x0f, 
	0xff, 0xf0, 0x00, 0x00, 0x03, 0xff, 0xc0, 0x00
};

// 'cat', 48x45px
const unsigned char catBitMap [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0f, 0x80, 0x70, 0x00, 0x00, 
	0x00, 0x0f, 0xe1, 0xf0, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xf0, 
	0x00, 0x00, 0x00, 0x1f, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x1b, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x10, 
	0xfc, 0x70, 0x00, 0x00, 0x00, 0x18, 0x78, 0x70, 0x00, 0x00, 0x00, 0x1c, 0x70, 0xf0, 0x00, 0x00, 
	0x00, 0x1e, 0x71, 0xf0, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xf0, 0x00, 0x00, 0x01, 0xff, 0xff, 0xf0, 
	0x00, 0x00, 0x07, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x1f, 0xff, 
	0xff, 0xf0, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xf0, 0x00, 0x00, 
	0x7f, 0xf0, 0xff, 0xf0, 0x07, 0x00, 0xff, 0xc0, 0x3f, 0xf0, 0x1f, 0xc0, 0xff, 0x80, 0x1f, 0xf0, 
	0x3f, 0xe0, 0xff, 0x00, 0x0f, 0xf0, 0x27, 0xe1, 0xfe, 0x00, 0x07, 0xf0, 0x01, 0xe1, 0xfe, 0x00, 
	0x07, 0xf0, 0x00, 0xf1, 0xfe, 0x00, 0x07, 0xf0, 0x00, 0xf1, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x71, 
	0xfe, 0x00, 0x00, 0x00, 0x00, 0x31, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x39, 0xfe, 0x00, 0x00, 0x00, 
	0x00, 0x38, 0xff, 0x00, 0x00, 0x00, 0x00, 0x18, 0xff, 0x00, 0x00, 0x00, 0x00, 0x1c, 0xff, 0x80, 
	0x00, 0x00, 0x00, 0x0c, 0x7f, 0xf0, 0x00, 0x00, 0x00, 0x0e, 0x7f, 0xff, 0xff, 0xe0, 0x00, 0x07, 
	0x3f, 0xff, 0xff, 0xe0, 0x00, 0x07, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x03, 0xff, 0xff, 0xff, 0xe0, 
	0x00, 0x00, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x0f, 0xff, 
	0xff, 0xe0, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

void setup() {
  Serial.begin(115200);
  pinMode (buzzer, OUTPUT) ;// set the digital IO pin mode,
  pinMode(buttonPin, INPUT);
  pinMode(buttonWiFiPin, INPUT);

//////////////////////////RGB
  pinMode(red_light_pin, OUTPUT);
  pinMode(green_light_pin, OUTPUT);
  pinMode(blue_light_pin, OUTPUT);
  
  ///////////////////DHT + OLED
  //For DHT11
  dht.begin();
  //For OLED I2C
  display.begin(i2c_Address, true); // Address 0x3C default
 //display.setContrast (0); // dim display
  display.clearDisplay();
  oledDisplayHeader(2, 28,0, "COM4RT");

  display.drawBitmap(42, 16, catBitMap, 48, 45, SH110X_WHITE); // display.drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color)
  display.display();

  display.display();  //can put our company logo
  delay(2000);
  // Clear the buffer.
  display.clearDisplay();
  // Initialize device.
  dht.begin();
  ///////////////////DHT + OLED

  wifiClient.setInsecure();  //the magic line, use with caution
  deviceID = parameter.getValue();
}

void loop() {
  // button1.tick();
  delay(100);
  humi = dht.readHumidity();
  tempC = dht.readTemperature();


  buttonMode();
  buttonWiFi();

  readingOutput();

  conditionDisplay();

  // display.clearDisplay();
  // oledDisplay(2,24,48,humi,"%");  //total size 36 hori 16 verti
  // oledDisplay(2,72,48,tempC,"C");//total size 36 + 8 = 44 
  // display.drawBitmap(0,30, humiBitMap, 24, 30, SH110X_WHITE); // display.drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color)
  // display.drawBitmap(60, 30, tempBitMap, 24, 30, SH110X_WHITE); // display.drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color)
  
  Serial.println(humi);
  Serial.println(tempC);
  display.display(); 
  String tempString = String(tempC, 1);
  String humdString = String(humi, 0);  
  postHTTP(url, tempString, humdString, deviceID);


}

void conditionDisplay(){
  if(tempC >= belowTempC && tempC < aboveTempC ){
    comfortDisplay();
  }
  else{uncomfortDisplay();}
}

void comfortDisplay(){  display.clearDisplay();
  display.clearDisplay();
  oledDisplay(2,24,48,humi,"%");  //total size 36 hori 16 verti
  oledDisplay(2,72,48,tempC,"C");//total size 36 + 8 = 44 
  display.drawBitmap(0,40, humiBitMap, 24, 24, SH110X_WHITE); // display.drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color)
  display.drawBitmap(60, 40, tempBitMap, 24, 24, SH110X_WHITE); // display.drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color)
  display.drawBitmap(44, 0, comfortBitMap, 40, 40, SH110X_WHITE);
    display.display();}

void uncomfortDisplay(){  display.clearDisplay();
  display.clearDisplay();
  oledDisplay(2,24,48,humi,"%");  //total size 36 hori 16 verti
  oledDisplay(2,72,48,tempC,"C");//total size 36 + 8 = 44 
  display.drawBitmap(0,40, humiBitMap, 24, 24, SH110X_WHITE); // display.drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color)
  display.drawBitmap(60, 40, tempBitMap, 24, 24, SH110X_WHITE); // display.drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color)
  display.drawBitmap(44, 0, uncomfortBitMap, 40, 40, SH110X_WHITE);
    display.display();}

void oledDisplayHeader(int size, int x,int y, String word){
 display.setTextSize(size);
 display.setTextColor(SH110X_WHITE);
 display.setCursor(x, y);
 display.print(word);}


void oledDisplay(int size, int x,int y, float value, String unit){
 int charLen=12;  //use 6 when set text size as 1, 12 for 2 and 18 for 3
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

void RGB_color(int red_light_value, int green_light_value, int blue_light_value){
  
  analogWrite(red_light_pin, red_light_value);
  analogWrite(green_light_pin, green_light_value);
  analogWrite(blue_light_pin, blue_light_value);}

void wifiManager(){   //set up only 
      manager.resetSettings();
      manager.setCustomHeadElement("COM4RT");   
      manager.setDebugOutput(false);      
      // manager.addParameter(&parameter);
      manager.setClass("invert");     // dark theme
      std::vector<const char*> menu = { "wifi", "exit" };
      manager.setMenu(menu);
      manager.autoConnect("COM4RT");
         if(manager.autoConnect("COM4RT")){
          Serial.println("connected...yeey :)");
          digitalWrite(buzzer, HIGH);
          delay(500);
          digitalWrite(buzzer, LOW);
          delay(500);
        }
        else {
            Serial.println("Configportal running");
        }
  }



void blinkLed(){
  RGB_color(255, 0, 0);  // red
  delay(1000);
  RGB_color(0, 0, 0);  // off
  delay(1000);}

void buzzerLight(){
  digitalWrite(buzzer, HIGH);
  delay(500);
  digitalWrite(buzzer, LOW);
  delay(500);}



void singleclick(){
  Serial.println("singlepressed");
  RGB_color(255, 0, 0);  // red
  delay(500);
  RGB_color(0, 0, 0);  // off
  delay(500);
  manager.disconnect();
  delay(500);
  if (WiFi.status() != WL_CONNECTED){ 
    delay(200); 
    Serial.print("disconnected...:(");  //press second time only disconnect bug
    }
  else{
    Serial.print("connected...yeey :)");
    digitalWrite(buzzer, HIGH);
    delay(500);
    digitalWrite(buzzer, LOW);
    delay(500);
    Serial.println(WiFi.localIP());
    Serial.println();
  }

  }


void longclick(){
  Serial.println("longpressed");
  RGB_color(255, 0, 0);  // red
  delay(500);
  RGB_color(0, 0, 0);  // off
  delay(500);
  manager.autoConnect("COM4RT");
  while (WiFi.status() != WL_CONNECTED){ 
    delay(200); 
    Serial.println("disconnected...:(");
    Serial.print("."); 
    }
  Serial.print("connected...yeey :)");
  digitalWrite(buzzer, HIGH);
  delay(500);
  digitalWrite(buzzer, LOW);
  delay(500);
  Serial.println(WiFi.localIP());
  Serial.println();
  }


void buttonMode(){
  int buttonStateMode = digitalRead(buttonWiFiPin);
  delay(550);
    // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonStateMode == HIGH) {  //havnt pressed
    digitalWrite(buzzer, LOW);
    delay(500);
    digitalWrite(buzzer, LOW);
    Serial.println("Off");
    //    sound();
  } else {
    // pressed:
    buttonPressStatus +=1; 
    Serial.println("Mode button on: "+ String(buttonPressStatus));
    Serial.print(buttonPressStatus);
    digitalWrite(buzzer, HIGH);
    RGB_color(255,0,0);
    delay(500);
    digitalWrite(buzzer, LOW);
    RGB_color(0,0,0);
  }
  delay(100);
  
  if (buttonPressStatus%2 == 0 and buttonPressStatus>0 ){  //should be connected
    manager.autoConnect("COM4RT");
      while (WiFi.status() != WL_CONNECTED){ 
        delay(200); 
        Serial.println("disconnected...:(");
        digitalWrite(buzzer, HIGH);  // if have buzzer means not work properly
        delay(500);
        digitalWrite(buzzer, LOW);
        delay(500);
        }
      Serial.print("connected...yeey :)");
      Serial.println(WiFi.localIP());
      Serial.println();
  }
  else if (buttonPressStatus%2 == 1 and buttonPressStatus>0){  //should be disconnected
    manager.disconnect();
      delay(500);
      if (WiFi.status() != WL_CONNECTED){ 
        delay(200); 
        Serial.print("disconnected...:(");  //press second time only disconnect bug
        }
      else{
      Serial.print("connected...yeey :)");
      Serial.println(WiFi.localIP());
      Serial.println();
      digitalWrite(buzzer, HIGH);  // if have buzzer means not work properly
      delay(500);
      digitalWrite(buzzer, LOW);
      delay(500);
      }}}
void postHTTP(String(url), String(tempReading), String(humdReading), String(deviceName)) {
  unsigned long delay_datasent = 3000;  // 10sec
  HTTPClient http;
  String response;

  http.begin(wifiClient, url);
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
  http.end();}
void buttonWiFi(){
  buttonState = digitalRead(buttonPin);
  delay(550);
  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState == HIGH) {
    digitalWrite(buzzer, LOW);
    delay(500);
    digitalWrite(buzzer, LOW);
    Serial.println("Off");
  //    sound();
  } else {
    // turn LED off:
    digitalWrite(buzzer, HIGH);
    RGB_color(255,0,0);
    delay(500);
    digitalWrite(buzzer, LOW);
    RGB_color(0,0,0);
    Serial.println("On");
    wifiManager();
  }}
void readingOutput(){
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
    }}

