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
#include <Target/LDRTarget.hpp>

LDRTarget::LDRTarget(uint8_t luminosityPin)
    : ambiantLuminosity(0), luminosityPin(luminosityPin), state(Ready),
      threshold(0) {

  state = ITarget::State::Activated;
}

void LDRTarget::post(ITarget::Event event) { nextEvent = event; }

void LDRTarget::update() {

  if (nextEvent != NoEvent) {

    switch (state) {

    case ITarget::State::Activated: {
      if (nextEvent == ITarget::Event::Calibrate) {
        onCalibrate();
      }
    }

    case ITarget::State::Calibrating: {
      if (nextEvent == ITarget::Event::Calibrated) {
        onReady();
      }
    }

    case ITarget::State::Ready: {
      if (nextEvent == ITarget::Event::Shoot) {
        onShoot();
      }
    }

    case ITarget::State::Hit: {
      if (nextEvent == ITarget::Event::Reset) {
        onReady();
      }
    }
    }

    if (nextEvent == ITarget::Event::Error) {
      onError();
    }

    // consumed or ignored
    nextEvent = NoEvent;
  } else {
    if (getLuminosity() > (ambiantLuminosity + threshold)) {
      post(ITarget::Event::Shoot);
    }
  }
}

ITarget::State LDRTarget::getState() { return state; }

uint16_t LDRTarget::getLuminosity() { return analogRead(luminosityPin); }

void LDRTarget::onError() { state = ITarget::State::Error; }
void LDRTarget::onReady() { state = ITarget::State::Ready; }
void LDRTarget::onShoot() { state = ITarget::State::Hit; }

void LDRTarget::onCalibrate() {
  state = ITarget::State::Calibrating;
  getLuminosity();
  ambiantLuminosity = getLuminosity();
  ambiantLuminosity += getLuminosity();
  ambiantLuminosity += getLuminosity();
  ambiantLuminosity += getLuminosity();
  ambiantLuminosity /= 4;
  post(ITarget::Event::Calibrated);
}

void LDRTarget::setThreshold(uint8_t threshold) { this->threshold = threshold; }
