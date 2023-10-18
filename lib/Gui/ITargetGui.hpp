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
#include <Player.hpp>

class ITargetGui {
public:

  enum TARGET{
    One = 0,
    Two,
    Three,
    Four,
    Five
  };
  
  /**
   * Tell GUI a target has been hit
  */
  virtual void hitTarget(TARGET) = 0;

  /**
   * Ask GUI if a target has been hit
  */
  virtual bool isTargetHit(TARGET) = 0;

  /**
   * Reset target hit status
  */
  virtual void resetTargets() = 0;

  virtual void setCurrentPlayer(uint8_t playerId) = 0;

  /*
  * Send player info : id, shoots and hit shoots
  *
  *
  */
  virtual void displayPlayerInfo(const Player&) = 0;

  virtual void restart() = 0;
  
};