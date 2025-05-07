#include <Arduino.h>


extern double rawTemp; //температура для анализа обвала температуры
extern volatile bool tempfail; //флаг для блокировки реле

double oldrawTemp = 0; //тут запоминаем с периодом 5 сек.

unsigned long sUpTime;
unsigned long ihour;
unsigned long imin;
unsigned long isec;
unsigned long iday;

//вычислить uptime д/ч/м/с (вызывается с периодом 5 сек)
void get_uptime(){
    sUpTime+=5;  //прибавляем 5 сек
    auto n=sUpTime; //количество всех секунд
    isec = n % 60;  //остаток от деления на 60 (секунд в минуте)
    n /= 60; //количество всех минут (целая часть)  
    imin = n % 60;  //остаток от деления на 60 (минут в часе)
    n /= 60; //количество всех часов (целая часть)
    ihour = n % 24; //остаток от деления на 24 (часов в дне)
    iday = n/24; //количество всех дней (целая часть)

    //устанавливаем блокировку по резкому обвалу температуры
    if (oldrawTemp>rawTemp and abs(oldrawTemp-rawTemp)>2){tempfail = true;}
    oldrawTemp = rawTemp;
}
