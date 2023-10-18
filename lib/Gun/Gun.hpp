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

#include <IGunHal.hpp>
#include <stdint.h>

class Gun {

  IGunHal &_hal;

public:
  uint8_t availableShots;
  Gun(IGunHal &hal) : _hal(hal), availableShots(0) {}
  void onButton1ShortPress();
  void onButton1LongPress();
  void onButton2ShortPress();
  void onButton2LongPress();
};