#include <Arduino.h>
#include <SPI.h>
#include "Adafruit_MAX31855.h"

#define MAXDO   16
#define MAXCS   15
#define MAXCLK  14

// initialize the Thermocouple
Adafruit_MAX31855 thermocouple(MAXCLK, MAXCS, MAXDO);

double currentTempRead = 0.0;

void setupSensor(){
    Serial.print("Initializing sensor...");
    if (!thermocouple.begin()) {
        Serial.println("ERROR.");
        while (1) delay(10);
    }
    Serial.println("DONE.");

    Serial.print("Internal Temp = ");
    Serial.println(thermocouple.readInternal());

    double c = thermocouple.readCelsius();
    if (isnan(c)) {
      Serial.println("Thermocouple fault(s) detected!");
      uint8_t e = thermocouple.readError();
      if (e & MAX31855_FAULT_OPEN) Serial.println("FAULT: Thermocouple is open - no connections.");
      if (e & MAX31855_FAULT_SHORT_GND) Serial.println("FAULT: Thermocouple is short-circuited to GND.");
      if (e & MAX31855_FAULT_SHORT_VCC) Serial.println("FAULT: Thermocouple is short-circuited to VCC.");
    } else {
      Serial.print("C = ");
      Serial.println(c);
    }
}

// получаем данные с термопары
double getCurrentTemperature(){
    double c = thermocouple.readCelsius(); //читаем термопару
    if (isnan(c)) {
      uint8_t e = thermocouple.readError();
      if (e & MAX31855_FAULT_OPEN) currentTempRead = -111; //ХХ
      if (e & MAX31855_FAULT_SHORT_GND) currentTempRead = -222; //КЗ земля
      if (e & MAX31855_FAULT_SHORT_VCC) currentTempRead = -333; //КЗ питание
      } else {
        currentTempRead = c;
    }
    return currentTempRead;
}