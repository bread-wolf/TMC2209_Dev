#include <Arduino.h>
#include "TMC2209_Lib.hpp"

#define CHIP_SERIAL  Serial2
#define CHIP_BR      115200
#define CHIP_ADD     0
#define CHIP_EN      21
TMC2209 TMC2209_Test(CHIP_SERIAL, CHIP_BR, CHIP_ADD, CHIP_EN);

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  if (!TMC2209_Test.begin())
  {
    // Turn on LED if chip isn't found.
    digitalWrite(LED_BUILTIN, HIGH);
  }
}

void loop() {

}