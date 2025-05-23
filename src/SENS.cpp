#include <Arduino.h>
#include <SPI.h>
#include "Adafruit_MAX31855.h"

#define SENS_PERIOD 150

extern double currentTemp; //температура термопары
extern bool overShootMode;
extern unsigned long time_now;

#define MAXDO   16  //MISO - data from slave  <--  (pull-up resistor 1-4.7 kom !!!)
#define MAXCS   15  //SS - serial select to slave -->
#define MAXCLK  14  //SC - serial clock to slave -->

// initialize the Thermocouple
//Adafruit_MAX31855 thermocouple(MAXCLK, MAXCS, MAXDO); //это программный SPI

SPIClass myspi1(HSPI); //если требуется несколько портов SPI..
Adafruit_MAX31855 thermocouple(MAXCS, &myspi1);

//Adafruit_MAX31855 thermocouple(MAXCS, &SPI); //это аппаратный SPI (ссылка на обьект SPI) !!!

int senserror = 0; //счетчик ошибок сенсора
int kind_error = -4; //вид ошибки сенсора
double rawTemp = 0; //температура датчика без обработки (без усреднения)
double filtTemp = 0; //фильтрованное значение температуры (IIR-фильтр)
//double arrTemp[5]; //массив для усреднения температуры (FIR-фильтр)
unsigned long sensCurrentTime = 0;  //для вычисления интервалов опросов датчика
unsigned long sensLastTime = 0;  //для вычисления интервалов опросов датчика

//инициализация сенсора
void setupSensor(){

    myspi1.begin(MAXCLK, MAXDO); //если требуется несколько портов SPI..
    //SPI.begin(MAXCLK, MAXDO/*, -1, -1*/); //создаем SPI со своими пинами !!!
    //SPI.beginTransaction(SPISettings(5000000, MSBFIRST, SPI_MODE0)); //скорость передачи данных 5 МГц (1 МГц умолч.)

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
  sensCurrentTime = time_now;
  //формируем период опроса датчика
  if(sensCurrentTime - sensLastTime >= SENS_PERIOD or sensLastTime > sensCurrentTime) {

    double c = thermocouple.readCelsius(); //читаем термопару
    if (!isnan(c)) { //нет ошибки
      rawTemp = c; //запоминаем температуру без обработки для TRMPFAIL

      //усреднение 5 элементов (0.5 сек.)
      //int elem = sizeof(arrTemp) / sizeof(arrTemp[0]);//колич. элем. в массиве
      //push_arr( arrTemp, elem, c );//сдвиг массива arrTemp влево
      //double roundTemp = sum_arr(arrTemp,elem)/elem; //усредняем знач.(FIR)

      filtTemp += (c - filtTemp) * 0.2; //фильтр (IIR)
      currentTemp = (round(filtTemp*100))/100;
      //currentTemp = c;
      //if (overShootMode) currentTemp = c; //агрессивный
      //else currentTemp = filtTemp;       //нормальный с фильтрованным значением

      if (senserror>=2) senserror=1;
      if (senserror==1) {senserror=0;kind_error = -4;}
    } else {
      uint8_t e = thermocouple.readError();
      if (e & MAX31855_FAULT_OPEN) kind_error = -1;
      if (e & MAX31855_FAULT_SHORT_GND) kind_error = -2;
      if (e & MAX31855_FAULT_SHORT_VCC) kind_error = -3;
      senserror++;
    }
    sensLastTime = sensCurrentTime;
  }
}
/*
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
*/
