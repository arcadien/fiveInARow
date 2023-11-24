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
#include <Player.hpp>

#include <stdint.h>

/**
 * A game has 4 players
 */
class Game {

  ITargetGui *gui;

public:
  static const uint8_t PLAYER_COUNT = 4;

  // clang-format off
  Player players[PLAYER_COUNT] = {
    Player(0), 
    Player(1),
    Player(2), 
    Player(3)
  };
  // clang-format on

  uint8_t currentRound;
  Player *currentPlayer;

  explicit Game(ITargetGui *);

  void recordSucceededShoot();

  /**
   * Change current player
   * playerIndex shall be [0;3]
   */
  void changeCurrentPlayerTo(uint8_t playerIndex);

  /**
   * End current player round, and initiate next player round
   */
  void nextRound();

  /**
   * Return true when all rounds have been played. A game stay finished until
   * restart.
   */
  bool isFinished();

  void reset();
};
