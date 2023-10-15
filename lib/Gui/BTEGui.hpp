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
#include <stdint.h>

#if defined(AVR)
#include <Arduino.h>
#else
#include <iostream>
#include <ostream>
#endif

static char stringBuffer[128];

/**
 * GUI for Bluetooth Electronics interface (Android)
 */
class BTEGui : public IGui {
  uint8_t targetState;

  /* Bluetooth application uses a letter to identify which widget
   * shall use the received data. Theses are for targets representation
   */
  static const char TARGET_APP_LETTERS[5];

  /* Bluetooth application uses a letter to identify which widget
   * shall use the received data. Theses are for player data representation.
   * Letters are organized by block of two: one for total shots, one for met
   * shots. ex. for player one, total is PLAYER_DATA_APP_LETTERS[0], and hit is
   * PLAYER_DATA_APP_LETTERS[1]. For player 2, total is
   * PLAYER_DATA_APP_LETTERS[2], and hit is PLAYER_DATA_APP_LETTERS[3] and so
   * on.
   */
  static const char PLAYER_DATA_APP_LETTERS[8];

  /**
   * Indirection to easy switch implementation
   * and does not pollute main code
   */
  void _output(const char *message) {
#if defined(AVR)
    Serial.println(stringBuffer);
#else
    out << message << "\n";
#endif
  }

public:
#if defined(AVR)
  BTEGui() : targetState(0) {}
#else
  std::ostream &out;
  BTEGui(std::ostream &out) : targetState(0), out(out) {}
#endif

  void hitTarget(IGui::TARGET target) {
    targetState |= (1 << target);
    char letter = TARGET_APP_LETTERS[(uint8_t)target];

    sprintf(stringBuffer, "*%cR255G255B255*", letter);

    _output(stringBuffer);
  }

  bool isTargetHit(IGui::TARGET target) {
    return ((targetState & (1 << target)) == (1 << target));
  }

  void resetTargets() { targetState = 0; }

  void displayPlayerInfo(const Player &player) {

    uint8_t baseIndex = player.id * 2;
    char totalLetter = PLAYER_DATA_APP_LETTERS[baseIndex];
    char hitLetter = PLAYER_DATA_APP_LETTERS[baseIndex + 1];

    sprintf(stringBuffer, "*%c%u*", totalLetter, unsigned(player.totalShoots));
    _output(stringBuffer);

    sprintf(stringBuffer, "*%c%u*", hitLetter,
            unsigned(player.getTotalHitCount()));
    _output(stringBuffer);
  }

#if defined(AVR)
  static void sendApplication() {
    Serial.println("*.kwl");
    Serial.println("clear_panel()");
    Serial.println("set_grid_size(21,10)");

    // targets
    Serial.println("add_led(2,1,2,A,0,0,0)");
    Serial.println("add_led(6,1,2,B,0,0,0)");
    Serial.println("add_led(10,1,2,C,0,0,0)");
    Serial.println("add_led(14,1,2,D,0,0,0)");
    Serial.println("add_led(18,1,2,E,0,0,0)");

    // Shoot row
    Serial.println("add_text_box(0,5,2,L,Shoot,245,240,245,)");
    Serial.println("add_text_box(4,5,1,L,0,245,240,245,Q)");
    Serial.println("add_text_box(8,5,1,L,0,245,240,245,S)");
    Serial.println("add_text_box(12,5,1,L,0,245,240,245,D)");
    Serial.println("add_text_box(16,5,1,L,0,245,240,245,F)");

    // Hit row
    Serial.println("add_text_box(0,6,2,L,Hit,245,240,245,)");
    Serial.println("add_text_box(4,6,1,L,0,245,240,245,W)");
    Serial.println("add_text_box(8,6,1,L,0,245,240,245,X)");
    Serial.println("add_text_box(12,6,1,L,0,245,240,245,C)");
    Serial.println("add_text_box(16,6,1,L,0,245,240,245,V)");

    // color buttons for players
    Serial.println("add_button(4,4,15,Y,y)");
    Serial.println("add_button(8,4,16,G,g)");
    Serial.println("add_button(12,4,14,R,r)");
    Serial.println("add_button(16,4,17,B,b)");

    Serial.println("add_text_box(18,4,3,L,Next,245,240,245,)");
    Serial.println("add_button(19,5,25,N,|)");

    Serial.println("add_button(0,8,30,R,|)");
    Serial.println("add_slider(10,9,8,200,800,449,T ,|,1)");

    Serial.println("add_monitor(18,8,3,,1)");
    Serial.println("set_panel_notes(-,,,)");
    Serial.println("run()");
    Serial.println("*");
  }
#else
  static void sendApplication() {}
#endif

  void restart() {
    sendApplication();
    for (char letter : TARGET_APP_LETTERS) {
      sprintf(stringBuffer, "*%cR0G0B0*", letter);
      _output(stringBuffer);
    }
  }
};

const char BTEGui::TARGET_APP_LETTERS[5] = {'A', 'B', 'C', 'D', 'E'};
const char BTEGui::PLAYER_DATA_APP_LETTERS[8] = {
    'Q', 'W', // player 1
    'S', 'X', // player 2
    'D', 'C', // player 3
    'F', 'V'  // player 4
};