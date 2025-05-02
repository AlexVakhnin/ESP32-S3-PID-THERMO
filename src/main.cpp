#include <Arduino.h>
#include <Ticker.h>

#define PID_INTERVAL 200  //(в милисекундах) период  для PID алгоритма

extern void disp_setup();
extern void initSPIFFS();
extern void web_init();
extern void wifi_init();
extern void get_uptime(); //работа Ticker - 5 сек.
extern void pwm_setup();
extern void pwm_handle();
extern void pid_setup();
extern bool pid_compute();
extern void updateCurrentTemperature();
extern void setHeatPowerPercentage(float power);
extern void disp_show();
extern void encoder_setup();
extern void encoder_handle();
extern int encoder_value();
extern void setupSensor();
extern String ds1;
extern String ds2;
extern double gOutputPwr; //результат вычислений PID
extern double gTargetTemp; //целевая температура
extern double currentTemp; //текущая температура по датчику
extern bool overShootMode; //далеко от цели..
extern int senserror; //счетчик ошибок термопары
extern void disp_refrash(); //обновить дисплей

//Для UpTime
Ticker hTicker;

//Global Variables
unsigned long sUpTime;
unsigned long isec = 0; //uptime: sec
unsigned long imin = 0; //uptime: min
unsigned long ihour = 0; //uptime: hour
unsigned long iday = 0; //uptime: day

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
  encoder_setup();
  setupSensor();

  delay(100);
  Serial.printf("Free heap after create objects:\t%d \r\n", ESP.getFreeHeap());

  time_now=millis();
  time_last=time_now;
}

void loop() {

  time_now=millis();
  updateCurrentTemperature(); //обновление текущей температуры с термопары (100 мс.)

  if(abs(time_now-time_last)>=PID_INTERVAL or time_last > time_now) { //обработка PID алгоритма T=200

    gTargetTemp = encoder_value(); //данные с энкодера

    if (pid_compute()){ //вычисляем..если результат PID готов.. 
      disp_refrash();
      setHeatPowerPercentage(gOutputPwr);  //задаем значение для PWM (0-1000)
    }
    time_last=time_now;
  }
  pwm_handle(); //обработчик PWM T=1000 
  encoder_handle(); //обработчик кнопки энкодера
}
