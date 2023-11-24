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
#include <Game.hpp>

// 5 rounds per player
static const uint8_t TOTAL_ROUNDS = 20;

Game::Game(ITargetUi *ui) {
  this->ui = ui;
  currentRound = 0;
  currentPlayer = &players[0];
}

void Game::recordSucceededShoot() {
  currentPlayer->recordSucceededShoot();
  ui->displayPlayerInfo(*currentPlayer);
}

/**
 * End current player round, and initiate next player round.
 * Note: loop on player 4
 */
void Game::nextRound() {

  currentPlayer->endRound();

  currentRound++;
  uint8_t playerId = currentPlayer->id;
  auto nextPlayerId = 0;
  if (playerId < (PLAYER_COUNT - 1)) {
    nextPlayerId = playerId + 1;
  }
  currentPlayer = &players[nextPlayerId];

  currentPlayer->startRound();
  ui->displayPlayerInfo(*currentPlayer);
}

bool Game::isFinished() {
  // -1 because rounds counts from 0 to 19
  return currentRound > TOTAL_ROUNDS - 1;
}

void Game::changeCurrentPlayerTo(uint8_t playerIndex) {
  if (playerIndex < PLAYER_COUNT) {
    currentPlayer->endRound();
    currentPlayer = &players[playerIndex];
    currentPlayer->startRound();
    ui->setCurrentPlayer(playerIndex);
    ui->resetTargets();
    ui->displayPlayerInfo(*currentPlayer);
  }
}

void Game::reset() {
  ui->restart();
  for (Player &player : players) {
    player.reset();
    ui->displayPlayerInfo(player);
  }
  currentRound = 0;
  currentPlayer = &players[0];
  currentPlayer->startRound();
  ui->displayPlayerInfo(*currentPlayer);
  ui->setCurrentPlayer(0);
}