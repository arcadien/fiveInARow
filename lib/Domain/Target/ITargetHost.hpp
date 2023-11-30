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
#include <stdint.h>

/**
 * @brief Target host is a device managing 5 targets
 *
 * A target is considered hit if its luminosity value exceed (ambientValue +
 * threshold) A target has a multicolor led: Off is ready, green is shot, blue
 * is calibration, red is error
 */

class ITargetHost {


public:

  virtual ~ITargetHost() {}

  virtual void reset() = 0;

  /**
   * Notification led on
   */
  virtual void ledOn() = 0;

  /**
   * Notification led off
   */
  virtual void ledOff() = 0;
};