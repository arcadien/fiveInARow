/*
 *
 * Copyright (c) 2023 Aurelien Labrosse
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

#include <iostream>
#include <string>

#include <Game.hpp>
#include <TargetHost/BTEGui.hpp>
#include <TargetHost/TargetHost.hpp>

void printLine() {
  std::cout << std::endl
            << "----------------------------------------------------"
            << std::endl;
}
int main() {
  std::cout << "Console Application" << std::endl;

  BTEGui gui(std::cout);
  Game game;
  TargetHost host(&game, &gui);

  printLine();

  gui.displayPlayerInfo(*game.currentPlayer);
  std::cout << "Type 'T' to fire a system tick" << std::endl;
  std::cout << "Type 'N' to switch to next player" << std::endl;
  std::cout << "Type 0,1,2,3 or 4 to hit a target" << std::endl;
  std::cout << "Type 'R' to reset the target host" << std::endl;
  std::cout << "Type 'Q' to quit game" << std::endl;

  while (true) {

    std::string str;
    std::getline(std::cin, str);

    if (str == "T") {
      host.update();

    } else if (str == "N") {
      game.nextRound();
      printLine();
      std::cout << "Player " << game.currentPlayer->id + 1
                << " turn! ( global round "
                << (game.currentRound / Game::PLAYER_COUNT) << ")" << std::endl;

    } else if (str == "R") {
      host.reset();

    } else if (str == "Q") {
      printLine();
      for (auto player : game.players) {
        gui.displayPlayerInfo(player);
      }
    }
  }
}