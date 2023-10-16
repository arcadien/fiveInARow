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
#include <IGui.hpp>
#include <cstdint>

class TestGui : public IGui {

public:
  uint8_t targetState;

  TestGui() : targetState(0) {}

  void hitTarget(IGui::TARGET target) override { targetState |= (1 << target); }

  bool isTargetHit(IGui::TARGET target) override {
    return (targetState & (1 << target) == (1 << target));
  }

  void setCurrentPlayer(uint8_t playerId) override {}
  void restart() override {}
  void resetTargets() override { targetState = 0; }
  void displayPlayerInfo(const Player &player) override {}
};