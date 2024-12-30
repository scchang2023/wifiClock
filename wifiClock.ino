#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include <WiFi.h>
#include <time.h>
#include "Timer.h"

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW/*PAROLA_HW*/
#define MAX_DEVICES 16

#define DATA_PIN  23  // or MOSI
#define CS_PIN    5  // or SS
#define CLK_PIN   18  // or SCK

// Hardware SPI connection
// MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);
// Arbitrary output pins
MD_Parola P = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

const char* ssid     = "LINKOU203-4F-2.4G";
const char* password = "56665666";
 
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 28800; //台灣時區+8hr,28800=8*60*60
const int   daylightOffset_sec = 0;  //台灣無日光節約時間

Timer t;

void setup(void)
{
  Serial.begin(115200);
  initLEDMatrix();
  if(P.displayAnimate()) {
    P.displayZoneText(1, "Wifi Con...", PA_CENTER, 0, 0, PA_NO_EFFECT, PA_NO_EFFECT);   
  }
  connectWifi();
  if(P.displayAnimate()) {
    P.displayZoneText(1, "Wifi OK", PA_CENTER, 0, 0, PA_NO_EFFECT, PA_NO_EFFECT);   
  }  
  syncNTPTime();
  //displayDateTime();
  t.every(1000, displayDateTime);
}

unsigned long lastSyncTime = 0; //上次同步時間
const unsigned long syncInterval = 1 * 3600 * 1000; //每1小時重新同步一次
void loop(void)
{
  unsigned long currentMillis = millis();
  if (currentMillis - lastSyncTime >= syncInterval && WiFi.status() == WL_CONNECTED) {
      syncNTPTime();
      lastSyncTime = currentMillis;
      Serial.println("NTP time re-synchronized.");
  }
  t.update();
}

void initLEDMatrix()
{
  P.begin(2);
  int endPart1 = (MAX_DEVICES/2);
  P.setZone(0, 0, endPart1-1);  //區域 0 控制第 0 到 3 號模組
  P.setZone(1, endPart1, MAX_DEVICES-1);  //區域 1 控制第 8 到 15 號模組
  //P.displayZoneText(0, "Good Job!", PA_CENTER, 200, 200, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
  //P.displayZoneText(1, "Hello World", PA_CENTER, 200, 200, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
}

void connectWifi()
{
  // Connect to Wi-Fi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
}

void syncNTPTime()
{
  // Init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();
}

void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %Y %m %d %H:%M:%S"); //%A-Sunday,%Y-2022,%m-3,%d-27,%H:%M:%S-21:10:02
  Serial.println("*****************************************************");
}

void displayDateTime()
{
  struct tm timeinfo;
  char timeBuf[16], dateBuf[16];
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    strcpy(timeBuf, "Loading time");
    strcpy(dateBuf, "");
  }else{
    //strftime(timeBuf, sizeof(buf), "%H:%M:%S", &timeinfo);
    strftime(timeBuf, sizeof(timeBuf), "%I:%M:%S %p", &timeinfo);  
    strftime(dateBuf, sizeof(dateBuf), "%m-%d %a", &timeinfo);  
  }
  Serial.println(timeBuf);
  Serial.println(dateBuf);  
  String currentTime = String(timeBuf);
  String currentDate = String(dateBuf);

  if(P.displayAnimate()) {
    updateDisplay(timeBuf, dateBuf);    
  }
}

String lastTime = "";
String lastDate = "";
void updateDisplay(char* timeBuf, char* dateBuf) {
  String currentTime = String(timeBuf);
  String currentDate = String(dateBuf);

  if (currentTime != lastTime) {
    P.displayZoneText(1, timeBuf, PA_CENTER, 0, 0, PA_NO_EFFECT, PA_NO_EFFECT);
    lastTime = currentTime;
  }
  if (currentDate != lastDate) {
    P.displayZoneText(0, dateBuf, PA_CENTER, 0, 0, PA_NO_EFFECT, PA_NO_EFFECT);
    lastDate = currentDate;
  }
}
