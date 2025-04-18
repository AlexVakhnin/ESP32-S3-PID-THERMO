#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

#define I2C_CLOCK 9
#define I2C_DATA 10
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

String ds1 = ""; //дисплей-строка 1
String ds2 = ""; //дисплей-строка 2

// Обновление состояния дисплея
void disp_show(){
  display.clearDisplay();
  display.setCursor(0,0); //координаты 1-й строки
  display.print(ds1);
  display.setCursor(0,18); //координаты 2-й строки
  display.print(ds2);
  display.display();//show

}
/*
// print "0"
void disp_0(){
    display.clearDisplay();
    display.display();//show
}

// print "1"
void disp_1(){
  display.clearDisplay();
  //display.drawRect(0,0,32,128,WHITE); //full region
  int y0= 43;

  display.fillRect(32-9,y0,9,85-2,WHITE);//43+85=128; 32-11+11=32; !!! 
  display.fillRect(13,y0+12,11,11,WHITE);
  display.fillTriangle(15, y0+12, 23, y0+12, 23, y0+3, WHITE);//X1,Y1,X2,Y2,X3,Y3
  display.fillTriangle(15, y0+22, 22, y0+22, 22,  y0+17, BLACK);//X1,Y1,X2,Y2,X3,Y3
  display.display();//show
}
// print "2"
void disp_2(){
  display.clearDisplay();
  //display.drawRect(0,43,32,83,WHITE);//рамка знакоместа

int y0= 43;
display.fillCircle(15, y0+15, 15, WHITE);  //внешний круг
display.fillCircle(15, y0+15, 7, BLACK);
display.fillTriangle(0, y0+30, 15, y0+15, 31,  y0+30, BLACK);//X1,Y1,X2,Y2,X3,Y3
display.fillTriangle(0, y0+30-5, 15, y0+15, 31,  y0+30-5, BLACK);//X1,Y1,X2,Y2,X3,Y3

display.fillTriangle(0, y0+83-8, 8, y0+83-8, 31-8+1,  y0+15+1, WHITE);//ножка
display.fillTriangle(31-8+1, y0+15+1,31-8+1+8,  y0+15+1, 8, y0+83-8, WHITE);//X1,Y1,X2,Y2,X3,Y3



display.fillRect(0,y0+83-8,32,8,WHITE); //подставка

//void drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);



display.display();//show
}
*/

//SSD1306 OLED Init
void disp_setup(){
  //delay(1000);
  //ReDefine I2C Pins
  Wire.begin(I2C_DATA, I2C_CLOCK);
  // Setup display SSD1306
  //delay(1000);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  //display.setRotation(3);
  display.display(); //show LOGO
  delay(1000);
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.clearDisplay();
  //display.setCursor(0, 0);
  //display.setTextSize(8);  
  //display.print("1");

  //display.drawRect(0,43,32,83,WHITE);
  //display.drawRect(0,0,32,32,WHITE);
  display.display();//show
  //delay(1000);
  //disp_1();
  //disp_2();

}