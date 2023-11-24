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

#include <BTEGui.hpp>

#if not defined(NATIVE)
#include <Arduino.h>
#endif

static char stringBuffer[64];

/**
 * Indirection to easy switch implementation
 * and does not pollute main code
 */
void BTEGui::_output(const char *message) {
#if not defined(NATIVE)
  Serial.println(stringBuffer);
#else
  out << message << "\n";
#endif
}

void BTEGui::hitTarget(ITargetUi::TARGET target) {
  targetState |= (1 << target);
  char letter = TARGET_APP_LETTERS[(uint8_t)target];

  sprintf(stringBuffer, "*%cR255G255B255*", letter);
  _output(stringBuffer);
}

bool BTEGui::isTargetHit(ITargetUi::TARGET target) {
  return ((targetState & (1 << target)) == (1 << target));
}

void BTEGui::resetTargets() {
  targetState = 0;
  for (char letter : TARGET_APP_LETTERS) {
    sprintf(stringBuffer, "*%cR0G0B0*", letter);
    _output(stringBuffer);
  }
}

void BTEGui::setCurrentPlayer(uint8_t playerId) {
  if (playerId < 4) {
    sprintf(stringBuffer, "*M%s*", PLAYER_COLORS[playerId]);
    _output(stringBuffer);
  }
}

void BTEGui::displayPlayerInfo(const Player &player) {

  uint8_t baseIndex = player.id * 2;
  char totalLetter = PLAYER_DATA_APP_LETTERS[baseIndex];
  char hitLetter = PLAYER_DATA_APP_LETTERS[baseIndex + 1];

  sprintf(stringBuffer, "*%c%u*", totalLetter, unsigned(player.totalShoots));
  _output(stringBuffer);

  sprintf(stringBuffer, "*%c%u*", hitLetter,
          unsigned(player.getTotalHitCount()));
  _output(stringBuffer);
}

void BTEGui::log(const char *message) {
  sprintf(stringBuffer, "%s", message);
  _output(stringBuffer);
}
void BTEGui::log(uint8_t value) {
  sprintf(stringBuffer, "%u", value);
  _output(stringBuffer);
}

#if not defined(NATIVE)
static void sendApplication() {
  Serial.println(F("*.kwl"));
  Serial.println(F("clear_panel()"));
  Serial.println(F("set_grid_size(21,10)"));

  // current player
  Serial.println(F("add_text(4,8,xlarge,L,Current player: , 245, 240, 245,)"));
  Serial.println(F("add_text(10,8,xlarge,C,1,245,240,245,M)"));

  // targets
  Serial.println(F("add_led(2,1,2,A,0,0,0)"));
  Serial.println(F("add_led(6,1,2,Z,0,0,0)"));
  Serial.println(F("add_led(10,1,2,E,0,0,0)"));
  Serial.println(F("add_led(14,1,2,R,0,0,0)"));
  Serial.println(F("add_led(18,1,2,T,0,0,0)"));

  // Shoot row
  Serial.println(F("add_text(0,5,large,L,Shoot,245,240,245,)"));
  Serial.println(F("add_text_box(4,5,1,L,0,245,240,245,Q)"));
  Serial.println(F("add_text_box(8,5,1,L,0,245,240,245,S)"));
  Serial.println(F("add_text_box(12,5,1,L,0,245,240,245,D)"));
  Serial.println(F("add_text_box(16,5,1,L,0,245,240,245,F)"));

  // Hit row
  Serial.println(F("add_text(0,6,large,L,Hit,245,240,245,)"));
  Serial.println(F("add_text_box(4,6,1,L,0,245,240,245,W)"));
  Serial.println(F("add_text_box(8,6,1,L,0,245,240,245,X)"));
  Serial.println(F("add_text_box(12,6,1,L,0,245,240,245,C)"));
  Serial.println(F("add_text_box(16,6,1,L,0,245,240,245,V)"));

  // color buttons for players
  Serial.println(F("add_button(4,4,15,,P 0 |)"));
  Serial.println(F("add_button(8,4,16,,P 1 |)"));
  Serial.println(F("add_button(12,4,14,,P 2 |)"));
  Serial.println(F("add_button(16,4,17,,P 3 |)"));

  Serial.println(F("add_text(19,4,large,L,Next,245,240,245,)"));
  Serial.println(F("add_button(19,5,25,N,|)"));

  Serial.println(F("add_button(0,8,30,R,|)"));
  Serial.println(F("add_slider(10,9,8,1,50,1,T ,|,1)"));

  Serial.println(F("add_monitor(18,8,3,,1)"));
  Serial.println(F("set_panel_notes(-,,,)"));
  Serial.println(F("run()"));
  Serial.println(F("*"));
}
#else
static void sendApplication() {}
#endif

void BTEGui::restart() {
  sendApplication();
  resetTargets();
}

const char *BTEGui::PLAYER_COLORS[] = {"Yellow", "Green", "Red", "Blue"};
const char BTEGui::TARGET_APP_LETTERS[5] = {'A', 'Z', 'E', 'R', 'T'};
const char BTEGui::PLAYER_DATA_APP_LETTERS[8] = {
    'Q', 'W', // player 1
    'S', 'X', // player 2
    'D', 'C', // player 3
    'F', 'V'  // player 4
};