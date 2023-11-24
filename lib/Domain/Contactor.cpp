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

#include <Contactor.hpp>

void Contactor::checkForLongPress(long now) {
  if ((downStartTime > 0) && ((now - downStartTime) >= LONG_PRESS)) {
    downStartTime = 0;
    onLongPress();
    inALongPress = true;
  }
}
void Contactor::onDown(long now) {
  currentState = State::Down;
  downStartTime = now;
}

void Contactor::onUp(long now) {
  currentState = State::Up;
  checkForLongPress(now);
  if (!inALongPress) {
    onShortPress();
  }
  inALongPress = false;
  downStartTime = 0;
}

void Contactor::processPendingEvent(long now) {
  Event toProcess = pendingEvent;
  pendingEvent = Event::NoEvent;
  if (toProcess == Event::Released) {
    onUp(now);
  } else if (toProcess == Event::Pressed) {
    onDown(now);
  }
}