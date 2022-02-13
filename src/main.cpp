// Arduino Due - Attempts to automatically figure out bus speed for each bus then show the trafic on the buses
// By Thibaut Viard/Wilfredo Molina/Collin Kidder 2013-2017
#include <Arduino.h>

#include "variant.h"
#include <due_can.h>
#include "can_sniffer.h"
#include <arduino-timer.h>

CanSniffer *sniffer; 
volatile bool initialize = false; 
auto timer = timer_create_default();

bool toggle_led(void *) {
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN)); // toggle the LED
  return true; // keep timer active? true
}


void setup()
{
  Serial.begin(250000);
  while (!Serial);
  sniffer = new CanSniffer();  
  pinMode(LED_BUILTIN, OUTPUT); // set LED pin to OUTPUT
  
}


void loop(){
  CAN_FRAME incoming;
  bool canMsgFound = false; 
  if (Can0.available() > 0) {
    canMsgFound = true; 
	  Can0.read(incoming); 
    sniffer->onCANReceive(incoming);
  }
  if (Can1.available() > 0) {
    Can1.read(incoming); 
    canMsgFound = true; 
    sniffer->onCANReceive(incoming);
  }

  if(canMsgFound && !initialize)
  {
    timer.every(1000, toggle_led);
    initialize = true;
  }
  timer.tick();
}
