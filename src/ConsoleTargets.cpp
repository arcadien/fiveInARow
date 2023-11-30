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

#include <target/BTEGui.hpp>
#include <Game.hpp>
#include <iostream>
#include <string>

void printLine() {
  std::cout << std::endl
            << "----------------------------------------------------"
            << std::endl;
}
int main() {
  std::cout << "Console Application" << std::endl;

  BTEGui gui(std::cout);
  Game game(&gui);

  printLine();

  gui.displayPlayerInfo(*game.currentPlayer);
  std::cout << "Type 'H' if a target is hit" << std::endl;
  std::cout << "Type 'N' to switch to next player" << std::endl;
  std::cout << "Type 'Q' to quit game" << std::endl;

  while (true) {
    std::string str;

    std::getline(std::cin, str);

    if (str == "H") {
      game.recordSucceededShoot();
     
    } else if (str == "Q") {
      printLine();
      for (auto player : game.players) {
        gui.displayPlayerInfo(player);
      }
      break;
    } else if (str == "N") {
      game.nextRound();
      printLine();
      std::cout << "Player " << game.currentPlayer->id + 1
                << " turn! ( global round " << (game.currentRound / Game::PLAYER_COUNT) << ")"
                << std::endl;
    }
  }
}