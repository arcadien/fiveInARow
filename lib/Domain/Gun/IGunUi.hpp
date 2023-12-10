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

#include <stdint.h>

class IGunUi {

public:
  virtual void setup() = 0;
  virtual void displaySplash(uint16_t timeoutMs) = 0;
  virtual void displayBatteryStatus(uint16_t mv, uint8_t percent) = 0;
  virtual void displayChargingStatus(bool isCharging) = 0;
  virtual void displayShootCount(uint16_t shootCount) = 0;
  virtual void displayCalibration() = 0;
  virtual void clearCalibration() = 0;
};