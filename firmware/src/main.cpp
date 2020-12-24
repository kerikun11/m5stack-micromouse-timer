/**
 * @file main.cpp
 * @author Ryotaro Onuki (kerikun11+github@gmail.com)
 * @brief MicroMouse Timer
 * @version 0.1
 * @date 2020-02-16
 * @copyright Copyright (c) 2020
 */
#include <M5Stack.h>
#include <WiFi.h>

#include "timer.h"

Timer t;

void show_boot_message() {
  M5.Lcd.setTextDatum(CC_DATUM);
  M5.Lcd.setTextFont(4);
  M5.Lcd.setTextSize(2);
  M5.Lcd.drawString("MicroMouse", M5.Lcd.width() / 2, M5.Lcd.height() / 2 - 60);
  M5.Lcd.drawString("Timer", M5.Lcd.width() / 2, M5.Lcd.height() / 2 + 0);
  M5.Lcd.drawString("@kerikun11", M5.Lcd.width() / 2, M5.Lcd.height() / 2 + 60);
}

void setup() {
  WiFi.mode(WIFI_OFF);
  M5.begin();
  /* boot */
  show_boot_message();
  /* wait for buttons pressed */
  while (!M5.BtnA.wasPressed() && !M5.BtnB.wasPressed() &&
         !M5.BtnC.wasPressed())
    M5.update();
  /* initialization */
  t.init();
  /* set time limit */
  t.time_select();
  /* timer begin */
  t.begin();
}

void loop() {
  M5.update();
  t.update();
  /* start line sensor */
  if (M5.BtnA.wasPressed())
    t.start_sensor();
  /* goal line sensor */
  if (M5.BtnB.wasPressed())
    t.goal_sensor();
  /* erase last one track */
  if (M5.BtnC.wasReleasefor(1000))
    t.erase();
  /* reset current time, and set to retry */
  if (M5.BtnC.wasReleased())
    t.touch();
}
