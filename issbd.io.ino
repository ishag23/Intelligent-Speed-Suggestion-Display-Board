// Include Libraries
#include "Arduino.h"
#include "DHT.h"
#include "LDR.h"
#include "HX711.h"
#include "Wire.h"
#include "SPI.h"
#include "Adafruit_SSD1306.h"
#include "Adafruit_GFX.h"
#include "RTClib.h"
// Pin Definitions
#define DHT_PIN_DATA 2
#define LDR_PIN_SIG A3
#define SCALE_PIN_DAT 4
#define SCALE_PIN_CLK 3
#define OLED128X64_PIN_RST 7
#define OLED128X64_PIN_DC 6
#define OLED128X64_PIN_CS 5
// Global variables and defines
#define THRESHOLD_ldr 100
int ldrAverageLight;
// object initialization
DHT dht(DHT_PIN_DATA);
LDR ldr(LDR_PIN_SIG);
HX711 scale(SCALE_PIN_DAT, SCALE_PIN_CLK);
#define calibration_factor 2280
#define SSD1306_LCDHEIGHT 64
Adafruit_SSD1306 oLed128x64(OLED128X64_PIN_DC, OLED128X64_PIN_RST, 
OLED128X64_PIN_CS);
RTC_DS3231 rtcDS;
// define vars for testing menu
const int timeout = 10000;
char menuOption = 0;
long time0;
void setup()
{
Serial.begin(9600);
while (!Serial) ;
Serial.println("start");
dht.begin();
ldrAverageLight = ldr.readAverage();
scale.set_scale(calibration_factor);
scale.tare();oLed128x64.begin(SSD1306_SWITCHCAPVCC);
oLed128x64.clearDisplay();
oLed128x64.display();
if (! rtcDS.begin()) {
Serial.println("Couldn't find RTC");
while (1);
}
if (rtcDS.lostPower()) {
Serial.println("RTC lost power, lets set the time!");
rtcDS.adjust(DateTime(F(__DATE__), F(__TIME__)));
}
menuOption = menu();
}
void loop()
{
if(menuOption == '1') {
float dhtHumidity = dht.readHumidity();
float dhtTempC = dht.readTempC();
Serial.print(F("Humidity: ")); Serial.print(dhtHumidity); 
Serial.print(F(" [%]\t"));
Serial.print(F("Temp: ")); Serial.print(dhtTempC); Serial.println(F(" 
[C]"));
oLed128x64.setTextSize(1);
oLed128x64.setCursor(0,0);
oLed128x64.print("Temperature: ");
oLed128x64.setTextSize(2);
oLed128x64.setCursor(0,10);
oLed128x64.print(t);
oLed128x64.print(" ");
oLed128x64.setTextSize(1);
oLed128x64.cp437(true);
oLed128x64.write(167);
oLed128x64.setTextSize(2);
oLed128x64.print("C");
oLed128x64.setTextSize(1);
oLed128x64.setCursor(0, 35);
oLed128x64.print("Humidity: ");
oLed128x64.setTextSize(2);
oLed128x64.setCursor(0, 45);
oLed128x64.print(h);
oLed128x64.print(" %");
oLed128x64.display();}
else if(menuOption == '2') {
int ldrSample = ldr.read();
int ldrDiff = abs(ldrAverageLight - ldrSample);
Serial.print(F("Light Diff: ")); Serial.println(ldrDiff);
oLed128x64.setTextSize(1);
oLed128x64.setTextColor(WHITE);
oLed128x64.setCursor(0, 10);
oLed128x64.clearDisplay();
oLed128x64.print("Light Diff:");
oLed128x64.setTextSize(2);
oLed128x64.setCursor(0,10);
oLed128x64.print(ldrDiff)
oLed128x64.display();
}
else if(menuOption == '3') {
float scaleUnits = scale.get_units();
Serial.print(scaleUnits);
Serial.println(" Kg");
oLed128x64.setTextSize(1);
oLed128x64.setTextColor(WHITE);
oLed128x64.setCursor(0, 10);
oLed128x64.clearDisplay();
oLed128x64.print(scaleUnits);
oLed128x64.setTextSize(2);
oLed128x64.setCursor(0,10);
oLed128x64.print(ldrDiff)
oLed128x64.display(" Kg");
}
else if(menuOption == '4') {
oLed128x64.setTextSize(1);
oLed128x64.setTextColor(WHITE);
oLed128x64.setCursor(0, 10);
oLed128x64.clearDisplay();
oLed128x64.print("Our Project Rocks!!");
oLed128x64.display();
delay(1);
oLed128x64.startscrollright(0x00, 0x0F);
delay(2000);
oLed128x64.stopscroll();
delay(1000);
oLed128x64.startscrollleft(0x00, 0x0F);
delay(2000);
oLed128x64.stopscroll();
}
else if(menuOption == '5') {DateTime now = rtcDS.now();
Serial.print(now.month(), DEC);
Serial.print('/');
Serial.print(now.day(), DEC);
Serial.print('/');
Serial.print(now.year(), DEC);
Serial.print(" ");
Serial.print(now.hour(), DEC);
Serial.print(':');
Serial.print(now.minute(), DEC);
Serial.print(':');
Serial.print(now.second(), DEC);
Serial.println();
delay(1000);
oLed128x64.setTextSize(1);
oLed128x64.setTextColor(WHITE);
oLed128x64.setCursor(0, 35);
oLed128x64.clearDisplay();
oLed128x64.print(now.month(), DEC);
oLed128x64.print('/');
oLed128x64.print(now.day(), DEC);
oLed128x64.print('/');
oLed128x64.print(now.year(), DEC);
oLed128x64.print(" ");
oLed128x64.print(now.hour(), DEC);
oLed128x64.print(':');
oLed128x64.print(now.minute(), DEC);
oLed128x64.print(':');
oLed128x64.print(now.second(), DEC);
oLed128x64.println();
}
if (millis() - time0 > timeout)
{
menuOption = menu();
}
}
char menu()
{
Serial.println(F("\nWhich component would you like to test?"));
Serial.println(F("(1) DHT22/11 Humidity and Temperature Sensor"));
Serial.println(F("(2) LDR (Mini Photocell)"));
Serial.println(F("(3) SparkFun HX711 - Load Cell Amplifier"));
Serial.println(F("(4) Monochrome 1.3 inch 128x64 OLED graphic 
display"));
Serial.println(F("(5) RTC - Real Time Clock"));
Serial.println(F("(menu) send anything else or press on board reset 
button\n"));while (!Serial.available());
while (Serial.available())
{
char c = Serial.read();
if (isAlphaNumeric(c))
{
if(c == '1')
Serial.println(F("Now Testing DHT22/11 
Humidity and Temperature Sensor"));
else if(c == '2')
Serial.println(F("Now Testing LDR (Mini 
Photocell)"));
else if(c == '3')
Serial.println(F("Now Testing SparkFun HX711 
- Load Cell Amplifier"));
else if(c == '4')
Serial.println(F("Now Testing Monochrome 1.3 
inch 128x64 OLED graphic display"));
else if(c == '5')
Serial.println(F("Now Testing RTC - Real Time 
Clock"));
else
{
Serial.println(F("illegal input!"));
return 0;
}
time0 = millis();
return c;
}
}
}
