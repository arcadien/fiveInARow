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
#include <Arduino.h>
#include <EEPROM.h>
#include <Target/TargetHost.hpp>
#include <Target/LDRTarget.hpp>
#include <SerialCommands.h>

char serial_command_buffer_[16];
SerialCommands _serial_commands(&Serial, serial_command_buffer_,
                                sizeof(serial_command_buffer_), "|", " ");

static const uint8_t TRESHOLD_ADDRESS = 0;
static const uint8_t LED_PIN = 9;
static const uint8_t TARGET_A_PIN = A0;
static const uint8_t TARGET_Z_PIN = A1;
static const uint8_t TARGET_E_PIN = A2;
static const uint8_t TARGET_R_PIN = A3;
static const uint8_t TARGET_T_PIN = A4;

/**
 * Fallback method called when garbage command
 * is received on the serial port
 */
static void serial_cmd_unrecognized_callback(SerialCommands *sender,
                                             const char *cmd);

static void serial_cmd_setThreshold_callback(SerialCommands *sender);
static void serial_cmd_changePlayer_callback(SerialCommands *sender);
static void serial_cmd_reset_callback(SerialCommands *sender);

SerialCommand serial_cmd_setThreshold("T", &serial_cmd_setThreshold_callback);
SerialCommand serial_cmd_changePlayer("P", &serial_cmd_changePlayer_callback);
SerialCommand serial_cmd_reset("R", &serial_cmd_reset_callback);

uint8_t TargetHost::getThreshold() {
  if (thresholdCache == 0) {
    thresholdCache = EEPROM.read(TRESHOLD_ADDRESS);
  }
  return thresholdCache;
}

void TargetHost::storeThreshold(uint8_t threshold) {
  thresholdCache = threshold;
  EEPROM.update(TRESHOLD_ADDRESS, threshold);
}

void TargetHost::ledOn() { digitalWrite(LED_PIN, HIGH); }

void TargetHost::ledOff() { digitalWrite(LED_PIN, LOW); }

void TargetHost::setup() {

  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  ledOff();

  // load value in cache from EEPROM
  getThreshold();

  for (ITarget *target : targets) {
    target->calibrate();
  }
  _serial_commands.context = this;
  _serial_commands.SetDefaultHandler(serial_cmd_unrecognized_callback);
  _serial_commands.AddCommand(&serial_cmd_setThreshold);
  _serial_commands.AddCommand(&serial_cmd_changePlayer);
  _serial_commands.AddCommand(&serial_cmd_reset);

  ui->restart();
}

void TargetHost::loop() {

  static const uint8_t targetCount = (sizeof(targets) / sizeof(ITarget));
  for (uint8_t targetIndex = 0; targetIndex < targetCount; targetIndex++) {
    ITarget *target = targets[targetIndex];
    if (target->getState() == ITarget::State::Hit) {
      ITargetUi::TARGET uiTarget;
      uiTarget = static_cast<ITargetUi::TARGET>(targetIndex);
      ui->hitTarget(uiTarget);
      ledOn();
      delay(100);
      ledOff();
    }
  }
  _serial_commands.ReadSerial();
  delay(5);
}

void serial_cmd_unrecognized_callback(SerialCommands *sender, const char *cmd) {
  sender->GetSerial()->print("Unrecognized command [");
  sender->GetSerial()->print(cmd);
  sender->GetSerial()->println("]");
}

void serial_cmd_reset_callback(SerialCommands *sender) {
  TargetHost *targetHost = static_cast<TargetHost *>(sender->context);
  targetHost->game->reset();
  targetHost->ui->restart();
}

void serial_cmd_setThreshold_callback(SerialCommands *sender) {

  uint8_t value = atoi(sender->Next());
  TargetHost *targetHost = static_cast<TargetHost *>(sender->context);

  if (value > 0 && value != targetHost->getThreshold()) {
    targetHost->storeThreshold(value);
    targetHost->ui->log((const char *)F("Thrsh: "));
    targetHost->ui->log(value);
    targetHost->ui->log("\n");
  }
}

void serial_cmd_changePlayer_callback(SerialCommands *sender) {
  uint8_t playerId = (uint8_t)atoi(sender->Next());
  TargetHost *targetHost = static_cast<TargetHost *>(sender->context);
  targetHost->game->changeCurrentPlayerTo(playerId);
}

void TargetHost::reset() { ui->resetTargets(); }

TargetHost::TargetHost(Game *game, ITargetUi *ui) {

  this->game = game;
  this->ui = ui;

  thresholdCache = 0;
  targets[0] = new LDRTarget(A0);
  targets[1] = new LDRTarget(A1);
  targets[2] = new LDRTarget(A2);
  targets[3] = new LDRTarget(A3);
  targets[4] = new LDRTarget(A4);
}