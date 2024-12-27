// Program to demonstrate the MD_Parola library
//
// Simplest program that does something useful - Hello World!
//
// MD_MAX72XX library can be found at https://github.com/MajicDesigns/MD_MAX72XX
//

#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

// Define the number of devices we have in the chain and the hardware interface
// NOTE: These pin numbers will probably not work with your hardware and may
// need to be adapted
//#define HARDWARE_TYPE MD_MAX72XX::PAROLA_HW
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 16

#define DATA_PIN  23  // or MOSI
#define CS_PIN    5  // or SS
#define CLK_PIN   18  // or SCK

// Hardware SPI connection
//MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);
// Arbitrary output pins
MD_Parola P = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

void setup(void)
{
  P.begin(2); 
  int endPart1 = (MAX_DEVICES / 2);
  P.setZone(0, 0, endPart1 - 1);  // 區域 0 控制第 0 到 3 號模組
  P.setZone(1, endPart1, MAX_DEVICES - 1);  // 區域 1 控制第 8 到 15 號模組

  //P.displayZoneText(0, "Hello", PA_CENTER, 100, 100, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
  P.displayZoneText(0, "Good Job!", PA_CENTER, 200, 200, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
  //P.displayZoneText(1, "World", PA_CENTER, 100, 100, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
  P.displayZoneText(1, "Hello World", PA_CENTER, 200, 200, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
}

void loop(void)
{
  // if (P.displayAnimate())
  //   P.displayText("e", PA_CENTER, P.getSpeed(), 1000, PA_NO_EFFECT, PA_NO_EFFECT);
  if (P.displayAnimate()) {
    P.displayReset(0);
    P.displayReset(1);
  }
}
