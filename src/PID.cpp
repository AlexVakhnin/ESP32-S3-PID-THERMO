#include <Arduino.h>
#include <PID_v1.h>

#define PID_INTERVAL 200  //(в милисекундах) период  для PID алгоритма

#define S_P 91.0
#define S_I 0.26
#define S_D 7950.0
#define S_aP 100.0
#define S_aI 0.0
#define S_aD 0.0
#define S_TSET 32.0
#define S_TBAND 1.5
#define BUF_SIZE 1024

extern unsigned long time_now;
extern unsigned long time_last;

double currentTemp = 0;

double gTargetTemp=200;//S_TSET; // 32.0
double gOvershoot=15;//S_TBAND; // 1.5  //Это Gap..
double gOutputPwr=0.0;
double gP = S_P, gI = S_I, gD = S_D; // 91.0, 0.26, 7950.0
double gaP = S_aP, gaI = S_aI, gaD = S_aD; // 100.0, 0.0, 0.0

bool overShootMode = false;

// создаем экземпляр ESPPID, 
// в конструктор передаем ссылки на переменные откуда брать значения и куда передавать результат
// gTargetTemp, currentTemp, gOutputPwr
// так же задаем коэффициенты для PID -> gP, gI, gD
PID ESPPID(&currentTemp, &gOutputPwr, &gTargetTemp, gP, gI, gD, DIRECT);

//----------------------------------------LOOP------------------------------
bool pid_handle(){

    double gap = abs(gTargetTemp-currentTemp); //distance away from setpoint
    if( !overShootMode && gap >= gOvershoot ) {     //ощибка по температуре > Gap   
        ESPPID.SetTunings(gaP, gaI, gaD); // 100.0, 0.0, 0.0 (форсаж)
        overShootMode=true;
    }
    else if( overShootMode && gap < gOvershoot ) {  //ощибка по температуре < Gap
        ESPPID.SetTunings(gP,gI,gD); // 91.0, 0.26, 7950.0 (норма)
        overShootMode=false;
    }
    return ESPPID.Compute(); //сама рещает, нужно ли вычислять, если вычисляет то true
}
//-------------------------------------END LOOP----------------------------

void pid_setup(){

  // start PID
  ESPPID.SetTunings(gP, gI, gD);
  ESPPID.SetSampleTime(PID_INTERVAL); //задаем период для вычислений PID
  ESPPID.SetOutputLimits(0, 1000);
  ESPPID.SetMode(AUTOMATIC);
}
