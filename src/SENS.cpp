#include <Arduino.h>
#include <SPI.h>
#include "Adafruit_MAX31855.h"

extern double currentTemp; //температура термопары
extern bool overShootMode;

#define MAXDO   16
#define MAXCS   15
#define MAXCLK  14

void push_arr( double arr[], int elem, double n );
double sum_arr( double arr[], int elem );

// initialize the Thermocouple
Adafruit_MAX31855 thermocouple(MAXCLK, MAXCS, MAXDO);

int senserror = 0; //счетчик ошибок сенсора
double arrTemp[5]; //массив для усреднения температуры

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

// получаем новые данные с термопары
// если ошибок нет - получаем новую температуру
// если ошибка, то оставляем старое значение
void updateCurrentTemperature(){
    double c = thermocouple.readCelsius(); //читаем термопару
    if (!isnan(c)) { //нет ошибки

      //усреднение 5 элементов (1 сек.)
      int elem = sizeof(arrTemp) / sizeof(arrTemp[0]);//колич. элем. в массиве
      push_arr( arrTemp, elem, c );//сдвиг массива arrTemp влево
      double roundTemp = sum_arr(arrTemp,elem)/elem; //усредняем знач.

      if (overShootMode) currentTemp = c; //агрессивный
      else currentTemp = roundTemp;       //нормальный

      if (senserror>=2) senserror=1;
      if (senserror==1) senserror=0;
    } else {
      senserror++;
    }
}

//сдвиг элементов массива влево с добавлением нового значения
void push_arr( double arr[], int elem, double n )
{
  for (int i=elem-1;i>0;i--)
    {
    //Serial.print(String(i)+",");
    arr[i] = arr[i-1];
    }
  arr[0] = n;
  //Serial.println(""); 
}

//сумма эдементов массива
double sum_arr( double arr[], int elem )
{
  double sum = 0;
  for (int i=0;i<elem;i++)
    {
    //Serial.print(String(i)+",");
    sum=sum+arr[i];
    }
    return sum;
}