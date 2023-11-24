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
#include <Gun/Gun.hpp>

#define TICKS_BETWEEN_BATTERY_UI_UPDATE 100

Gun::Gun(IGunHal *hal, IGunUi *ui) {
  this->hal = hal;
  this->ui = ui;
  calibrationMode = false;
  shootCount = 0;

  // display at first loop
  shootCycleCountdown = 0;
  batteryDisplayCycleCountdown = 0;

  button.setGun(this);
  trigger.setGun(this);
}

void Gun::toggleCalibrationMode() {
  calibrationMode = !calibrationMode;
  if (calibrationMode) {
    hal->laserOn();
    hal->vibrationOff();
    ui->displayCalibration();
  } else {
    hal->laserOff();
    ui->clearCalibration();
    ui->displayShootCount(shootCount);
    batteryDisplayCycleCountdown = 0;
  }
}

void Gun::resetShoots() {
  shootCount = 0;
  ui->displayShootCount(shootCount);
}

void Gun::activateShoot() {
  if (!calibrationMode && shootCycleCountdown == 0) {
    shootCount += 1;
    hal->laserOn();
    hal->vibrationOn();

    // add 1 to allow easy detection of last tick
    shootCycleCountdown = Gun::SHOOT_DURATION_TICKS + 1;
  }
}

void Gun::loop(void) {

  // 10ms per loop thanks to timer2
  millisSinceStart += 10;

  trigger.processPendingEvent(millisSinceStart);
  button.processPendingEvent(millisSinceStart);

  button.checkForLongPress(millisSinceStart);

  if (shootCycleCountdown > 0) {
    shootCycleCountdown--;
    if (shootCycleCountdown == 1) {
      hal->laserOff();
      hal->vibrationOff();
      ui->displayShootCount(shootCount);
      shootCycleCountdown--;
    }
  }

  if (batteryDisplayCycleCountdown > 0) {
    batteryDisplayCycleCountdown--;
  } else {
    ui->displayBatteryStatus(hal->getBatteryVoltageMv(),
                             hal->getBatteryVoltagePercent());
    batteryDisplayCycleCountdown = TICKS_BETWEEN_BATTERY_UI_UPDATE;
  }
}

void Gun::setup(void) {
  ui->displaySplash(2000);
  // mV is not used
  ui->displayBatteryStatus(0, hal->getBatteryVoltagePercent());
  ui->displayShootCount(0);
}