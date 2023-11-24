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

#include <Game.hpp>
#include <ITargetUi.hpp>
#include <ITargetHost.hpp>
#include <stdint.h>

/**
 * Manages a collection of targets, the game logic and a GUI.
 * Also manage the `threshold`, used to differentiate ambient
 * luminosity level and 'hit' luminosity level.
 */
class TargetHost : public ITargetHost {

  /**
   * Minimum light value to validate a hit.
   * Shall be stored in EEPROM to allow a reset
   * in same luminosity condition without reconfiguring
   */
  uint8_t thresholdCache;
  
public:
  Target targets[5];
  Game *game;
  ITargetUi *ui;

  TargetHost(Game *game, ITargetUi *ui);

  /* ITargetHost API */

  uint16_t getLightIntensityFor(const Target &target) override;
  void ledOn() override;
  void ledOff() override;
  void resetTargets() override;
  uint8_t getThreshold() override;

  /* Specific methods */

  void setup();
  void loop();
  void storeThreshold(uint8_t threshold);

};