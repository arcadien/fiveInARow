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

// forward declaration
class Gun;

class IGunHal {
public:
  virtual ~IGunHal() {}

  virtual void setGun(Gun *gun) = 0;

  /*
   * the 'loop' method shall be called each 10ms
   */
  virtual void setupHeartbeat() = 0;
  virtual bool triggerIsUp() = 0;
  virtual bool buttonIsUp() = 0;
  virtual void laserOn() = 0;
  virtual void laserOff() = 0;
  virtual void vibrationOn() = 0;
  virtual void vibrationOff() = 0;
  virtual uint16_t getBatteryVoltageMv() = 0;
  virtual uint8_t getBatteryVoltagePercent() = 0;
  virtual bool isCharging() = 0;
  virtual void sleep() = 0;
  virtual void configureInputCallbacks() = 0;
};