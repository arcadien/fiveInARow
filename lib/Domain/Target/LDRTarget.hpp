
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
#include <Target/ITarget.hpp>

// forward declaration
class TargetHost;

class LDRTarget : public ITarget {

  uint16_t ambiantLuminosity;
  uint8_t luminosityPin;
  ITarget::State state;
  ITarget::Event nextEvent;
  uint8_t threshold;

  uint16_t getLuminosity();
  void onReady();
  void onError();
  void onShoot();
  void onCalibrate();

  enum State { All, Activated, Calibrating, Ready, Hit, Failure };
  enum Event { NoEvent, Calibrate, Calibrated, Shoot, Reset, Error };

public:
  LDRTarget(uint8_t luminosityPin);

  ITarget::State getState() override;

  void post(ITarget::Event event) override;

  void update() override;

  /**
   * @brief Set the difference with ambiant luminosity level that triggers a hit
   * condition
   *
   * @param threshold
   */
  void setThreshold(uint8_t threshold);
};