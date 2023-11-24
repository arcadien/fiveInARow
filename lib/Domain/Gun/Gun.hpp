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

#include <Contactor.hpp>
#include <Gun/Button.hpp>
#include <Gun/IGunHal.hpp>
#include <Gun/IGunUi.hpp>
#include <Gun/Trigger.hpp>

class Gun {

  uint8_t shootCycleCountdown;
  uint16_t batteryDisplayCycleCountdown;
  long millisSinceStart = 0;
  bool calibrationMode;

public:
  /* 50 ms */
  static const uint8_t SHOOT_DURATION_TICKS = 5;

  IGunHal *hal;
  IGunUi *ui;
  Button button;
  Trigger trigger;
  uint16_t shootCount;

  Gun(IGunHal *hal, IGunUi *ui);

  /**
   * Reset shoot count and redisplay
   */
  void resetShoots();

  /**
   * Activate laser and vibrator
   */
  void activateShoot();

  void toggleCalibrationMode();

  /**
   * @brief Gun loop shall be called at least each 10ms,
   *        but interrupt shall trigger loop() execution in
   *        a shorter delay.
   *
   */
  void loop();

  void setup();
};