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

#include <ITargetGui.hpp>
#include <stdint.h>

#if not defined(AVR)
#include <ostream>
#endif

/**
 * GUI for Bluetooth Electronics interface (Android)
 */
class BTEGui : public ITargetGui {
  uint8_t targetState;

  /* Bluetooth application uses a letter to identify which widget
   * shall use the received data. Theses are for targets representation
   */
  static const char TARGET_APP_LETTERS[5];

  /* Bluetooth application uses a letter to identify which widget
   * shall use the received data. Theses are for player data representation.
   * Letters are organized by block of two: one for total shots, one for met
   * shots. ex. for player one, total is PLAYER_DATA_APP_LETTERS[0], and hit is
   * PLAYER_DATA_APP_LETTERS[1]. For player 2, total is
   * PLAYER_DATA_APP_LETTERS[2], and hit is PLAYER_DATA_APP_LETTERS[3] and so
   * on.
   */
  static const char PLAYER_DATA_APP_LETTERS[8];

  /**
   * On that GUI, each 4 player has a color
   */
  static const char *PLAYER_COLORS[4];

  void _output(const char *message);

public:
#if defined(AVR)
  BTEGui() : targetState(0) {}
#else
  std::ostream &out;
  BTEGui(std::ostream &out) : targetState(0), out(out) {}
#endif

  void setCurrentPlayer(uint8_t playerId) override;
  void hitTarget(ITargetGui::TARGET target) override;
  bool isTargetHit(ITargetGui::TARGET target) override;
  void resetTargets() override;
  void displayPlayerInfo(const Player &player) override;
  void restart() override;
};