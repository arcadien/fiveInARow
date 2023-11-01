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
#include <Player.hpp>

Player::Player(uint8_t id) : id(id), currentRound(0), totalShoots(0) {
  reset();
}

uint8_t Player::getTotalHitCount() const {
  uint8_t accumulator = 0;
  for (uint8_t index = 0; index < ROUND_COUNT; index++) {
    accumulator += hit[index];
  }
  return accumulator;
}

void Player::reset() {
  // Note: this loop uses less asm code than memset() (release -Os)
  for (uint8_t index = 0; index < ROUND_COUNT; index++) {
    hit[index] = 0;
  }
  currentRound = 0;
  totalShoots = 0;
}

void Player::startRound() {
  totalShoots += 5;
}

void Player::endRound() {
  currentRound = 0;
  if (currentRound < (ROUND_COUNT - 1)) {
    currentRound++;
  }
}

void Player::recordSucceededShoot() {
  if (this->hit[currentRound] < MAX_HIT_IN_A_ROUND) {
    this->hit[currentRound] += 1;
  }
}
