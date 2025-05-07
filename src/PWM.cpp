#include <Arduino.h>

#define PWM_PERIOD 1000  //период PWM
#define IND_PIN  33 //порт для индикации включения реле
#define RELAY_PIN  34 //порт управления реле

extern unsigned long time_now;
extern int senserror; //счетчик ошибок термопары
extern double currentTemp;
extern double rawTemp; //температура без обработки

bool overheat = false; //перегрев
volatile bool tempfail = false; //резкий обвал температуры (датчик физически отпал..)

float heatcycles = 0; //время в милисекундах верхней полки PWM (0-1000) - мощность нагрева !!!
//float mem_heatcycles = 0; //запоминаем длительность импульса в начале периода
bool pwmState = 0; //содержит текущее состояние выхода PWM (0/1)
unsigned long heatCurrentTime = 0;  //для вычисления интервалов PWM
unsigned long heatLastTime = 0;  //для вычисления интервалов PWM

void pwm_setup() {
    pinMode(RELAY_PIN, OUTPUT); //управление реле
    digitalWrite(RELAY_PIN, false);

    pinMode(IND_PIN, OUTPUT); //индикация включения реле
    digitalWrite(IND_PIN, false);
}

//меняем состояние нагревателя,
//применяем аварийные блокировки
void _turnHeatElementOnOff(bool state) {

    if(senserror!=0) state = 0; //блокировка при аварии датчика !!!
    if(rawTemp>=360) overheat=true;//блокировка по высокой температуре !!!
    //if(rawTemp<50) overheat=false;
    if(overheat) state = 0;
    if(tempfail) state = 0;

    digitalWrite(RELAY_PIN, state); //реле
    digitalWrite(IND_PIN, state); //индикация
    pwmState = state;
}

//формирование PWM, вызывается из loop()
void pwm_handle() {
  heatCurrentTime = time_now;
  bool newStatus = false;
  bool shouldUpdate = false;

  //формируем период PWM (T=1 сек.)
  if(heatCurrentTime - heatLastTime >= PWM_PERIOD or heatLastTime > heatCurrentTime) { //второй оператор предотвращает ошибки переполнения
    // начинаем новый период PWM !!!
    //mem_heatcycles = heatcycles; //
    shouldUpdate = true; //нужно обновить
    newStatus = true;  //обновить - включить
    //Serial.println("__/-- "+String(heatCurrentTime)+"-"+String(heatLastTime)+"="+String(heatCurrentTime - heatLastTime));
    heatLastTime = heatCurrentTime;
  }

  // формируем длительность импульса PWM
  if (heatCurrentTime - heatLastTime >= /*mem_*/heatcycles &&  pwmState==1) {
    shouldUpdate = true; //нужно обновить
    newStatus = false;  //обновить - выключить
    //Serial.println("--\\__ "+String(heatCurrentTime)+"-"+String(heatLastTime)+"="+String(heatCurrentTime - heatLastTime));
  }

  //меняем состояние выхода PWM (0/1)
  if(shouldUpdate){
    _turnHeatElementOnOff(newStatus);
  }
}

//Устанавливаем мощность нагрева (0-1000)
void setHeatPowerPercentage(float power) {
  if (power < 0.0) {
    power = 0.0;
  }  
  if (power > 1000.0) {
    power = 1000.0;
  }
  //установка мощности нагрева в милисекундах !!! (0-1000)
  heatcycles = power;
}

float getHeatCycles() {
  return heatcycles;
}
