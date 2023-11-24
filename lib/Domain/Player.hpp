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
 * A player has 5 cycling rounds
 */
class Player {

public:
  static const uint8_t ROUND_COUNT = 5;
  static const uint8_t MAX_HIT_IN_A_ROUND = 5;

  const uint8_t id;
  uint8_t hit[ROUND_COUNT] = {0, 0, 0, 0, 0};
  uint8_t currentRound;
  uint8_t totalShoots;

  explicit Player(uint8_t id);

  uint8_t getTotalHitCount() const;

  void reset();
  void startRound();
  void endRound();
  void recordSucceededShoot();
};