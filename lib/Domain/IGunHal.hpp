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

class IGunHal {
public:
  virtual void isButton1Pressed() = 0;
  virtual void isButton2Pressed() = 0;
  virtual void shortDelay() = 0;
  virtual void longDelay() = 0;
  virtual void ledOn() = 0;
  virtual void ledOff() = 0;
  virtual void laserOn() = 0;
  virtual void laserOff() = 0;
  virtual void vibrationOn() = 0;
  virtual void vibrationOff() = 0; 
  virtual void deepSleep() = 0; 
};