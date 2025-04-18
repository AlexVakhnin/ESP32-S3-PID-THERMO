#include <Arduino.h>
#include <Ticker.h>
//-----------------
//#include "main.h"   //определяет функции во внешних файлах
//Это внешние функции для main.cpp(слово "extern" можно опускать)
extern void disp_setup();
extern void initSPIFFS();
extern void web_init();
extern void wifi_init();

extern void get_uptime(); //работа Ticker - 5 сек.
//-----------------

//Для UpTime
Ticker hTicker;

//Global Variables
unsigned long sUpTime;
unsigned long isec = 0; //uptime: sec
unsigned long imin = 0; //uptime: min
unsigned long ihour = 0; //uptime: hour
unsigned long iday = 0; //uptime: day
//String formatted_time = "--:--:--";
const int analogPin = 6;
int analogValue = 0;
int mapVal = 9;

void setup() {

   Serial.begin(115200);

  //PSRAM Initialisation
  if(psramInit()){
        Serial.println("\nThe PSRAM is correctly initialized");
  }else{
        Serial.println("\nPSRAM does not work");
  }
  //OLED SSD1306 128X32
  disp_setup();

  Serial.println("-----------------------------------------");
  Serial.printf("Total heap:\t%d \r\n", ESP.getHeapSize());
  Serial.printf("Free heap:\t%d \r\n", ESP.getFreeHeap());
  Serial.printf("Total PSRAM:\t%d \r\n", ESP.getPsramSize());
  Serial.printf("Free PSRAM:\t%d \r\n", ESP.getFreePsram());
  Serial.printf("Flash size:\t%d (bytes)\r\n", ESP.getFlashChipSize());
  Serial.println("I2C_SDA= "+String(SDA));
  Serial.println("I2C_SCL= "+String(SCL));
  Serial.println("-----------------------------------------");

  initSPIFFS(); //инициализация SPIFFS

    //инициализация прерывания (5 sec.)
  hTicker.attach_ms(5000, get_uptime);


  //Инициализация WIFI
  wifi_init();

  // Route for root / web page
  web_init();

  delay(100);
  Serial.printf("Free heap after create objects:\t%d \r\n", ESP.getFreeHeap());

  analogReadResolution(9);

  pinMode(33, OUTPUT); 
  digitalWrite(33, false);
}


void loop() {

  analogValue = analogRead(analogPin);
  mapVal = map(analogValue, 0, 511, 100, 350);
  Serial.println(mapVal);

digitalWrite(33, true);
  delay(500);
  digitalWrite(33, false);
  delay(500);

}
