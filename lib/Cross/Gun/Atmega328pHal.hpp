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

#include <Gun/IGunHal.hpp>

class Atmega328pHal : public IGunHal {
  Gun *gun;

public:
  ~Atmega328pHal() {}

  Atmega328pHal();

  /*
   * the 'loop' method shall be called each 10ms
   *
   * Configure timer2 so that an interrupt occurs each 10ms
   * and wake up the MCU if it is in sleep mode.
   */
  void setupHeartbeat() override;

  void setGun(Gun *gun) override;
  void laserOn() override;
  void laserOff() override;
  void vibrationOn() override;
  void vibrationOff() override;
  bool triggerIsUp() override;
  bool buttonIsUp() override;
  uint16_t getBatteryVoltageMv() override;
  uint8_t getBatteryVoltagePercent() override;
  bool isCharging() override;
  void configureInputCallbacks() override;
  void sleep() override;
};