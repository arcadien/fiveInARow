#pragma once

#include <stdint.h>

/**
 * A player has 5 cycling rounds
 */
struct Player {
  static const uint8_t ROUND_COUNT = 5;
  const uint8_t id;
  uint8_t met[ROUND_COUNT] = {0, 0, 0, 0, 0};
  uint8_t currentRound;

  Player(uint8_t id) : id(id), currentRound(0) {}

  uint8_t getTotalMetCount() {
    uint8_t accumulator = 0;
    for (uint8_t index = 0; index < ROUND_COUNT; index++) {
      accumulator += met[index];
    }
    return accumulator;
  }

  void reset() {
    for (uint8_t index = 0; index < ROUND_COUNT; index++) {
      met[index] = 0;
    }
    currentRound = 0;
  }
  void nextRound() {
    if (currentRound < ROUND_COUNT) {
      currentRound++;
    } else {
      currentRound = 0;
    }
  }
  void recordSucceededShoot() { met[currentRound] += 1; }
};

/**
 * A game has 4 players
 */
struct Game {
  static const uint8_t MAX_PLAYER = 4;
  Player players[MAX_PLAYER] = {Player(0), Player(1), Player(2), Player(3)};
  Player *currentPlayer;

  Game() { currentPlayer = &players[0]; }

  void recordSucceededShoot() { currentPlayer->recordSucceededShoot(); }

  /**
   * End current player round, and initiate next player round
   */
  void nextRound() {
    if (currentPlayer->id < MAX_PLAYER) {
      currentPlayer = &players[currentPlayer->id + 1];
    } else {
      currentPlayer = &players[0];
    }
    currentPlayer->nextRound();
  }

  void restart() {
    for (uint8_t index = 0; index < 4; index++) {
      players[index].reset();
    }
    currentPlayer = &players[0];
  }
};
