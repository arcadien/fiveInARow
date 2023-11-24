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

/**
 * A target is considered hit if its
 * luminosity value exceed (ambientValue + threshold)
 */
class Target {
public:
  uint16_t ambientValue;
  bool isHit;
  uint8_t pin;
  uint8_t index;
  Target() : ambientValue(0), isHit(false), pin(0) {}
};

class ITargetHost {
public:
  virtual ~ITargetHost() {}

  /**
   * Return light intensity for given target
   * value [0;1024]
   */
  virtual uint16_t getLightIntensityFor(const Target &target) = 0;

  /**
   * Return value of difference needed between ambient light level and hit light
   * level value [0;254]
   */
  virtual uint8_t getThreshold() = 0;

  virtual void resetTargets() = 0;

  /**
   * Notification led
   */
  virtual void ledOn() = 0;

  /**
   * Notification led
   */
  virtual void ledOff() = 0;
};