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
      threshold(0) {}

void LDRTarget::reset() { state = Ready; }

void LDRTarget::calibrate() {
  state = Calibrating;
  getLuminosity();
  ambiantLuminosity = getLuminosity();
  ambiantLuminosity += getLuminosity();
  ambiantLuminosity += getLuminosity();
  ambiantLuminosity += getLuminosity();
  ambiantLuminosity /= 4;
  reset();
}

boolean LDRTarget::check() {
  return (getLuminosity() > (ambiantLuminosity + threshold));
}

void LDRTarget::hit() { state = Hit; }

void LDRTarget::setThreshold(uint8_t threshold) { this->threshold = threshold; }

uint16_t LDRTarget::getLuminosity() { 
  return analogRead(luminosityPin); 
}

ITarget::State LDRTarget::getState() { return state; }