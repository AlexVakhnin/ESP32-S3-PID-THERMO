#include <Arduino.h>
#include <WiFi.h>


#define CLK_PIN 6
#define DT_PIN 7
#define SW_PIN 8

volatile int counter = 0;
int lastStateCLK;

//Обработка прерывания
void IRAM_ATTR rotary_encoder() {
    int currentStateCLK = digitalRead(CLK_PIN);
    if (currentStateCLK != lastStateCLK) {
        if (digitalRead(DT_PIN) != currentStateCLK) {
            counter++;
        } else {
            counter--;
        }
        Serial.print("Position: ");
        Serial.println(counter);
    }
    lastStateCLK = currentStateCLK;
}

void encoder_setup() {
    pinMode(CLK_PIN, INPUT);
    pinMode(DT_PIN, INPUT);
    pinMode(SW_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(CLK_PIN), rotary_encoder, CHANGE);
    //Serial.begin(115200);
    //Serial.println("KY-040 Rotary Encoder Test");
}

void encoder_handle() {
    if (digitalRead(SW_PIN) == LOW) {
        Serial.println("Button Pressed");
        //delay(100);
    }
}