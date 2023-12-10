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

#include <Arduino.h>
#include <TargetHost/TargetHost.hpp>
#include <Game/Basic5/Basic5.hpp>
#include <Game/Basic5/Basic5Ui.hpp>


Basic5Ui ui;
Basic5 game(&ui);
TargetHost host(&game);

void setup() {
  host.setup();
  game.restart();
}

void loop() { host.update(); }