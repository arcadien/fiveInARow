/*
 *
 * Copyright (c) 2023 Aurélien Labrosse
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

#include <Gun/IGunUi.hpp>

#include <Adafruit_SSD1306.h>
#include <Fonts/FreeMonoBold18pt7b.h>
#include <Fonts/FreeMonoOblique9pt7b.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET -1    // Reset pin # (or -1 if sharing Arduino reset pin)
// 0x3D for 128x64, 0x3C for 128x32
#define SCREEN_ADDRESS 0x3C

static Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

/**
 * @brief UI on a SSD1306 OLED display wired over I²C bus
 *
 * Used https://rickkas7.github.io/DisplayGenerator/index.html for prototyping
 */
class SSD1306Ui : public IGunUi {
public:
  SSD1306Ui() {}

  void setup() override {
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
      for (;;)
        ; // Don't proceed, loop forever
    }
  }

  void displaySplash(uint16_t timeoutMs) override {
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.setFont(&FreeMonoOblique9pt7b);
    display.setCursor(9, 11);
    display.println("5 in a row");
    display.setCursor(4, 27);
    display.println(" # # # # #");
    display.display();
    delay(timeoutMs);
    display.clearDisplay();
    display.display();
  }

  void displayBatteryStatus(uint16_t mv, uint8_t percent) override {
    display.setFont(NULL);
    display.fillRect(100, 0, 25, 9, 0); // clear
    display.setCursor(100, 2);
    display.print(percent);
    display.println("%");
  }

  void displayChargingStatus(bool isCharging) override {
    if (isCharging) {
      display.setFont(NULL);
      display.setCursor(80, 15);
      display.println("charging");
    } else {
      display.fillRect(80, 12, 47, 11, 0); // clear
    }
  }

  void displayShootCount(uint16_t count) override {
    display.fillRect(11, 0, 68, 30, 0); // clear
    display.setFont(&FreeMonoBold18pt7b);
    if (count > 999) {
      count = 0;
    }
    if (count < 10) {
      display.setCursor(15, 25);
    } else {
      display.setCursor(10, 25);
    }
    display.print(count);
    display.display();
  }

  void clearCalibration() override {
    display.fillRect(2, 10, 126, 22, 0); // clear
  }

  void displayCalibration() override {
    display.clearDisplay();
    display.setFont(&FreeMonoOblique9pt7b);
    display.setCursor(2, 25);
    display.print("Calibration");
    display.display();
  }
};