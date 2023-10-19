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

#include <BTEGui.hpp>
#include <Game.hpp>
#include <SerialCommands.h>

#define TARGET_A_PIN A0
#define TARGET_Z_PIN A1
#define TARGET_E_PIN A2
#define TARGET_R_PIN A3
#define TARGET_T_PIN A4

#define LED_PIN 9

char serial_command_buffer_[32];
SerialCommands serial_commands_(&Serial, serial_command_buffer_,
                                sizeof(serial_command_buffer_), "|", " ");
void cmd_unrecognized(SerialCommands *sender, const char *cmd);
void cmd_resetTargets(SerialCommands *sender);
void cmd_setThreshold(SerialCommands *sender);
void cmd_nextRound(SerialCommands *sender);
void cmd_changePlayer(SerialCommands *sender);

SerialCommand cmd_nextRound_("N", &cmd_nextRound);
SerialCommand cmd_resetTargets_("R", &cmd_resetTargets);
SerialCommand cmd_setThreshold_("T", &cmd_setThreshold);
SerialCommand cmd_changePlayer_("P", &cmd_changePlayer);

void serialPrintInfo(uint16_t value);
void ledOn();
void ledOff();

// laser detection threshold
uint16_t threshold;

// ambient light ADC value
uint16_t reference;

BTEGui gui;
Game game(&gui);

void setup() {

  pinMode(LED_PIN, OUTPUT);
  ledOff();

  pinMode(TARGET_A_PIN, INPUT);
  pinMode(TARGET_Z_PIN, INPUT);
  pinMode(TARGET_E_PIN, INPUT);
  pinMode(TARGET_R_PIN, INPUT);
  pinMode(TARGET_T_PIN, INPUT);

  serial_commands_.SetDefaultHandler(cmd_unrecognized);
  serial_commands_.AddCommand(&cmd_resetTargets_);
  serial_commands_.AddCommand(&cmd_setThreshold_);
  serial_commands_.AddCommand(&cmd_nextRound_);
  serial_commands_.AddCommand(&cmd_changePlayer_);

  analogRead(TARGET_A_PIN);
  analogRead(TARGET_Z_PIN);
  analogRead(TARGET_E_PIN);
  analogRead(TARGET_R_PIN);
  analogRead(TARGET_T_PIN);

  reference = 0;
  
  reference += analogRead(TARGET_A_PIN);
  reference += analogRead(TARGET_Z_PIN);
  reference += analogRead(TARGET_E_PIN);
  reference += analogRead(TARGET_R_PIN);
  reference += analogRead(TARGET_T_PIN);
  reference /= 5;

  // initial value sent by GUI at startup
  threshold = 500; 

  Serial.begin(115200);
  
  game.reset();
  serial_commands_.ReadSerial();
}

static void _recordHit() {
  game.recordSucceededShoot();
  ledOn();
  delay(100);
  ledOff();
}

static void _checkHit(uint16_t value, ITargetGui::TARGET target) {
  if (value > threshold) {
    if (!gui.isTargetHit(target)) {
      gui.hitTarget(target);
      _recordHit();
    }
  }
}

void loop() {

  uint16_t value1 = analogRead(A0);
  uint16_t value2 = analogRead(A1);
  uint16_t value3 = analogRead(A2);
  uint16_t value4 = analogRead(A3);
  uint16_t value5 = analogRead(A4);

  _checkHit(value1, ITargetGui::TARGET::One);
  _checkHit(value2, ITargetGui::TARGET::Two);
  _checkHit(value3, ITargetGui::TARGET::Three);
  _checkHit(value4, ITargetGui::TARGET::Four);
  _checkHit(value5, ITargetGui::TARGET::Five);

  serial_commands_.ReadSerial();
  delay(5);
}

void serialPrintInfo(uint16_t value) {
  if (value > 0) {
    Serial.print(F("HV: "));
    Serial.println(value);
  }
}
void cmd_unrecognized(SerialCommands *sender, const char *cmd) {
  sender->GetSerial()->print("Unrecognized command [");
  sender->GetSerial()->print(cmd);
  sender->GetSerial()->println("]");
}
void cmd_setThreshold(SerialCommands *sender) {
  uint16_t value = atoi(sender->Next());
  if (value > 0 && value != threshold) {
    threshold = value;
    Serial.print(F("VThresh: "));
    Serial.println(threshold);
    Serial.print(F("Vref: "));
    Serial.println(reference);
  }
}
void cmd_changePlayer(SerialCommands *sender) {
  uint8_t playerId = (uint8_t)atoi(sender->Next());
  game.changeCurrentPlayerTo(playerId);
}
void cmd_resetTargets(SerialCommands *sender) {

  (void)sender;
  game.reset();
  serialPrintInfo(0);
  ledOn();
  delay(200);
  ledOff();
  delay(200);
  ledOn();
  delay(200);
  ledOff();
}
void cmd_nextRound(SerialCommands *sender) {
  gui.resetTargets();
  game.nextRound();
  gui.setCurrentPlayer(game.currentPlayer->id);
}

void ledOff() { digitalWrite(LED_PIN, HIGH); }
void ledOn() { digitalWrite(LED_PIN, LOW); }