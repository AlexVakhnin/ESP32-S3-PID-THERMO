#include <Arduino.h>
#include <Ticker.h>
//#include <U8g2lib.h>
//#include "ws2812b.h"

//-----------------
//#include "main.h"   //определяет функции во внешних файлах
//Это внешние функции для main.cpp(слово "extern" можно опускать)
extern void disp_setup();
//void disp_2();
//void disp_1();
//void disp_0();
extern void initSPIFFS();
extern void web_init();
extern void wifi_init();

//extern void time_init();
//extern void handle_time();
extern void get_uptime(); //работа Ticker - 5 сек.
//-----------------
/*
//Управдение RGB светодиодной лентой
#define GPIO_RGB_BUILTIN_LED 7

//SPI for SH1106 OLED 128x64
#define SPI_CLOCK 12  //33
#define SPI_DATA 11   //34
#define SPI_CS 18
#define SPI_DC 17
#define SPI_RESET 35

U8G2_SH1106_128X64_NONAME_F_4W_HW_SPI u8g2(U8G2_R0, SPI_CS, SPI_DC, SPI_RESET); //Work with fspi !
//U8G2_SH1106_128X64_NONAME_F_4W_SW_SPI u8g2(U8G2_R0, SPI_CLOCK, SPI_DATA, SPI_CS, SPI_DC, SPI_RESET); //Work !

//class ws2812b
ws2812b ws;
*/

//Для UpTime
Ticker hTicker;

//Global Variables
unsigned long sUpTime;
unsigned long isec = 0; //uptime: sec
unsigned long imin = 0; //uptime: min
unsigned long ihour = 0; //uptime: hour
unsigned long iday = 0; //uptime: day
//String formatted_time = "--:--:--";

void setup() {

   Serial.begin(115200);

  //PSRAM Initialisation
  if(psramInit()){
        Serial.println("\nThe PSRAM is correctly initialized");
  }else{
        Serial.println("\nPSRAM does not work");
  }
/*
  //OLED SH1106 128x64
  u8g2.begin();
  u8g2.clearBuffer();					// clear the internal memory
  u8g2.setFont(u8g2_font_ncenB10_tr);	// choose a suitable font
  u8g2.drawStr(0,10+4,"Connecting WIFI");	// write something to the internal memory
  u8g2.sendBuffer();					// transfer internal memory to the display
*/
  //OLED SSD1306 128X32
  disp_setup();

/*
  //ws2812b strip
  ws.begin(GPIO_RGB_BUILTIN_LED,5);
  ws.color(0,1,2,3);
  ws.show();
*/
 
  Serial.println("-----------------------------------------");
  Serial.printf("Total heap:\t%d \r\n", ESP.getHeapSize());
  Serial.printf("Free heap:\t%d \r\n", ESP.getFreeHeap());
  Serial.printf("Total PSRAM:\t%d \r\n", ESP.getPsramSize());
  Serial.printf("Free PSRAM:\t%d \r\n", ESP.getFreePsram());
  Serial.printf("Flash size:\t%d (bytes)\r\n", ESP.getFlashChipSize());
  Serial.println("I2C_SDA= "+String(SDA));
  Serial.println("I2C_SCL= "+String(SCL));
 // Serial.println("SPI_SCK= "+String(SCK));
 // Serial.println("SPI_MOSI= "+String(MOSI));
 // Serial.println("SPI_MISO= "+String(MISO));
 // Serial.println("SPI_SS= "+String(SS));
  Serial.println("-----------------------------------------");

  initSPIFFS(); //инициализация SPIFFS

    //инициализация прерывания (5 sec.)
  hTicker.attach_ms(5000, get_uptime);


  //Инициализация WIFI
  wifi_init();

  //NTP Client START
  //time_init();

  // Route for root / web page
  web_init();

  delay(100);
  Serial.printf("Free heap after create objects:\t%d \r\n", ESP.getFreeHeap());
}

//uint8_t m = 0;

void loop() {
/*
handle_time(); // NTP Time -> formatted_time `hh:mm:ss`

String smin = formatted_time.substring(3,5);
String s2hours = formatted_time.substring(1,2);
String s1hours = formatted_time.substring(0,1);
//первая цифра
if(s1hours == "2"){
    disp_2();
} else if(s1hours == "1"){
    disp_1();
} else {
    disp_0();
}
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_logisoso62_tn);
    u8g2.drawStr(0,63,s2hours.c_str());
    u8g2.drawStr(33,63,":");
    u8g2.drawStr(50,63,smin.c_str());
  } while ( u8g2.nextPage() );

  ws.test(m);  //LED STRIP SHOW !
 
  //Serial.println(m);
  delay(1000);
  m++;
  if ( m == 5 )
    m = 0;
  */  
}
