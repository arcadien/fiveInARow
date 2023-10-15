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

#define LIGHT_MEASURE_PIN A0
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
  pinMode(LIGHT_MEASURE_PIN, INPUT);
  serial_commands_.SetDefaultHandler(cmd_unrecognized);
  serial_commands_.AddCommand(&cmd_resetTargets_);
  serial_commands_.AddCommand(&cmd_setThreshold_);
  serial_commands_.AddCommand(&cmd_nextRound_);

  analogRead(LIGHT_MEASURE_PIN);
  reference = analogRead(LIGHT_MEASURE_PIN);
  threshold = 500;

  Serial.begin(115200);
  Serial.println("--- Laser target beta ---");
  delay(1000);
  game.restart();
  gui.restart();
}

void loop() {
  uint16_t value = analogRead(A0);
  if (value > threshold) {
    serialPrintInfo(value);
    game.recordSucceededShoot();
    ledOn();
    delay(500);
    ledOff();
  } else {
    serial_commands_.ReadSerial();
    delay(10);
  }
}

void serialPrintInfo(uint16_t value) {
  if (value > 0) {
    Serial.print(F("HV: "));
    Serial.println(value);
  }
  Serial.print(F("TV: "));
  Serial.println(threshold);
  Serial.print(F("RV: "));
  Serial.println(reference);
}
void cmd_unrecognized(SerialCommands *sender, const char *cmd) {
  sender->GetSerial()->print("Unrecognized command [");
  sender->GetSerial()->print(cmd);
  sender->GetSerial()->println("]");
}
void cmd_setThreshold(SerialCommands *sender) {
  char *threshold_value = sender->Next();
  uint16_t value = atoi(threshold_value);
  if (value > 0)
    threshold = value;
  Serial.print(F("TV: "));
  Serial.println(threshold);
}

void cmd_changePlayer(SerialCommands *sender) {
  uint8_t playerId = (uint8_t) int(sender->Next());
  // game.changeCurrentPlayerTo(playerId);
  game.nextRound();
  Serial.print(F("Player changed to :"));
  Serial.println(playerId);
}

void cmd_resetTargets(SerialCommands *sender) {

  (void)sender;

  ledOn();

  game.restart();

  serialPrintInfo(0);
  delay(2000);
  ledOff();
}
void cmd_nextRound(SerialCommands *sender) { game.nextRound(); }

void ledOff() { digitalWrite(LED_PIN, HIGH); }
void ledOn() { digitalWrite(LED_PIN, LOW); }