
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
 * This interface suggests an implemetation following the state pattern.
 * `update()` behaviour can change according to current ITarget state.
 *
 */
class ITarget {

public:

  enum State { Activated, Calibrating, Ready, Hit, Error };
  enum Event { NoEvent, Calibrate, Calibrated, Shoot, Reset };

  virtual ~ITarget() {}

  virtual void post(ITarget::Event event) = 0;

  virtual ITarget::State getState() = 0;

  /**
   * System heartbeat callback
   */
  virtual void update() = 0;
};