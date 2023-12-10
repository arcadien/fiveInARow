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

#include <Gun/Atmega328pHal.hpp>
#include <Gun/Gun.hpp>
#include <Gun/SSD1306Ui.hpp>

SSD1306Ui ui;
Atmega328pHal hal;
Gun gun(&hal, &ui);

volatile bool applicativeEvent = false;
ISR(TIMER2_COMPA_vect) { applicativeEvent = true; }

volatile Contactor::Event pendingTriggerEvent;
void triggerInterruptHandler() {
  if (Contactor::Event::NoEvent == pendingTriggerEvent) {
    if (hal.triggerIsUp()) {
      pendingTriggerEvent = Contactor::Event::Released;
    } else {
      pendingTriggerEvent = Contactor::Event::Pressed;
    }
    applicativeEvent = true;
  }
}

volatile Contactor::Event pendingButtonEvent;
void buttonInterruptHandler() {
  if (Contactor::Event::NoEvent == pendingButtonEvent) {
    if (hal.buttonIsUp()) {
      pendingButtonEvent = Contactor::Event::Released;
    } else {
      pendingButtonEvent = Contactor::Event::Pressed;
    }
    applicativeEvent = true;
  }
}

void loop(void) {

  // other interrupt may wakeup the board
  // and trigger loop(). Especially counter0
  // and counter1, used internally by the Arduino
  // framework. Avoid processing these wakeups as
  // applicative events using 'tick' control variable.

  if (applicativeEvent) {
    applicativeEvent = false;

    // wire interrupt-based events with main code
    gun.trigger.pendingEvent = pendingTriggerEvent;
    gun.button.pendingEvent = pendingButtonEvent;

    gun.loop();

    pendingTriggerEvent = Contactor::Event::NoEvent;
    pendingButtonEvent = Contactor::Event::NoEvent;

    hal.sleep();
  }
}

void setup(void) {
  hal.setGun(&gun);
  ui.setup();
  gun.setup();
  hal.configureInputCallbacks();
  hal.setupHeartbeat();
}