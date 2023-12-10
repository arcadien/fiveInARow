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

#include <Game/IGame.hpp>
#include <Game/IGameUi.hpp>

/**
 * This game has a single player, who have unlimited
 * ammunition to shot the five targets.
 */
class Basic5 : public IGame {

  IGameUi *ui;
  Player player;

public:
  Basic5(IGameUi *ui) : player(0) { this->ui = ui; }

  ~Basic5() {}

  Player *getCurrentPlayer();

  void restart() {
    ui->restart();
    player.restart();
  }

  void changeCurrentPlayerTo(uint8_t playerId) {
    // only one player
  }
};