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
#include <Gun.hpp>

static const uint8_t SHOTS_PER_REARM = 5;

void Gun::onButton1ShortPress() {
  if (availableShots > 0) {
    _hal.laserOn();
    _hal.vibrationOn();
    _hal.shortDelay();
    _hal.laserOff();
    _hal.vibrationOff();
    availableShots--;
  }
  
}

void Gun::onButton1LongPress() {
    _hal.deepSleep();
}
void Gun::onButton2ShortPress() { availableShots = SHOTS_PER_REARM; }
void Gun::onButton2LongPress() {
    _hal.laserOn();
}
