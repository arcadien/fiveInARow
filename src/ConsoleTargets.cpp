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

#include <Game/basic5/Basic5.hpp>
#include <Game/basic5/Basic5Ui.hpp>
#include <TargetHost/TargetHost.hpp>
#include <iostream>

void printLine() {
  std::cout << std::endl
            << "----------------------------------------------------"
            << std::endl;
}
int main() {
  std::cout << "Console Application" << std::endl;

  Basic5Ui ui(std::cout);
  Basic5 game(&ui);
  TargetHost host(&game);

  game.restart();

  printLine();

  std::cout << "Type 'T' to fire a system tick" << std::endl;
  // std::cout << "Type 'N' to switch to next player" << std::endl;
  std::cout << "Type 0,1,2,3 or 4 to hit a target" << std::endl;
  std::cout << "Type 'R' to reset the target host" << std::endl;
  std::cout << "Type 'Q' to quit game" << std::endl;

  while (true) {

    std::string str;
    std::getline(std::cin, str);

    if (str == "T") {
      host.update();

    } else if (str == "N") {

    } else if (str == "0") {

    } else if (str == "1") {

    } else if (str == "2") {

    } else if (str == "3") {

    } else if (str == "4") {

    } else if (str == "R") {
      host.restart();

    } else if (str == "Q") {
      exit(0);
    }
  }
}