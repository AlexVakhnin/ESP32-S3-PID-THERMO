
//внешние переменные

extern unsigned long sUpTime;
extern unsigned long ihour;
extern unsigned long imin;
extern unsigned long isec;
extern unsigned long iday;


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
}
