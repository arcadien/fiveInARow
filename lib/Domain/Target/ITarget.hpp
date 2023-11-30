
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

class ITarget {

public:
  enum State { Calibrating, Ready, Hit, Error };

  virtual ~ITarget() {}

  /**
   * @brief transition to Ready state
   *
   */
  virtual void reset() = 0;

  /**
   * @brief transition to Calibration state
   *
   */
  virtual void calibrate() = 0;

  /**
   * @brief transition to Hit state
   *
   */
  virtual void hit() = 0;

  virtual uint16_t getLuminosity() = 0;
  
  virtual State getState() = 0;
};