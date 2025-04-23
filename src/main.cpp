#include <Arduino.h>
#include <Ticker.h>

#define PID_INTERVAL 200  //(в милисекундах) период  для PID алгоритма
//-----------------
//#include "main.h"   //определяет функции во внешних файлах
//Это внешние функции для main.cpp(слово "extern" можно опускать)
extern void disp_setup();
extern void initSPIFFS();
extern void web_init();
extern void wifi_init();
extern void get_uptime(); //работа Ticker - 5 сек.
extern void pwm_setup();
extern void pwm_handle();
extern void pid_setup();
extern bool pid_compute();
extern double getCurrentTemperature();
extern void setHeatPowerPercentage(float power);
extern void disp_show();
//extern int target_val;
extern String ds1;
extern String ds2;
extern double gOutputPwr; //результат вычислений PID
extern double gTargetTemp; //целевая температура
extern double currentTemp; //текущая температура по датчику
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
//int analogValue = 0;
//int mapVal = 9;

long time_now=0; //текущее время в цикле
long time_last=0; //хранит аремя для периодического события PID алгоритма


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
  pwm_setup();
  pid_setup();

  delay(100);
  Serial.printf("Free heap after create objects:\t%d \r\n", ESP.getFreeHeap());

  analogReadResolution(9);

  pinMode(33, OUTPUT); 
  digitalWrite(33, false);

  time_now=millis();
  time_last=time_now;
}


void loop() {
  time_now=millis();

  if(abs(time_now-time_last)>=PID_INTERVAL or time_last > time_now) { //обработка PID алгоритма T=200

    int analogValue = analogRead(analogPin); //считываем регулятор
    gTargetTemp = (int)map(analogValue, 0, 511, 0, 290); //переводим датчик в нужный диапазон
    currentTemp = getCurrentTemperature(); //данные с термопары

    if (pid_compute()){ //вычисляем..если результат PID готов.. 
      ds1=String((int)gTargetTemp)+"  "+String((int)currentTemp);ds2=String(gOutputPwr);disp_show(); //результат на дисплей
      setHeatPowerPercentage(gOutputPwr);  //задаем значение для PWM (0-1000)
    }

    time_last=time_now;
    //Serial.println("Event-PID Computing.."+String(counter++)+" "+String(heatcycles));
  }


  pwm_handle(); //обработчик PWM T=1000
}
