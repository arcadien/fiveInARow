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

/**
 * Designed to manage hardware buttons and their different behaviour:
 * - short press
 * - long press
 */
class Contactor {

  static const int LONG_PRESS = 2000;
  long downStartTime = 0;
  bool inALongPress = false;

public:
  enum Event { NoEvent, Pressed, Released };
  enum State { Up, Down };

  /**
   * @brief Event can be detected in an interrupt, and processed later
   *
   */
  Event pendingEvent;

  State currentState;

  Contactor() : pendingEvent(Event::NoEvent), currentState(State::Up) {}

  virtual void checkForLongPress(long now);
  virtual void onDown(long now);
  virtual void onUp(long now);
  virtual void processPendingEvent(long now);

  virtual ~Contactor() {}

  virtual void onShortPress(){};
  virtual void onLongPress(){};
};