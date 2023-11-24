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
#include <Gun/Atmega328pHal.hpp>
#include <LowPower.h>
#include <avr/io.h>

#define BSP_TICKS_PER_SEC 100
#define F_CPU 16000000L

#define LASER_PIN 10
#define VIBRATOR_PIN 6
#define BATTERY_VOLTAGE_PIN A3
#define CHARGING_STATE_PIN A2
#define BUTTON_PIN 3
#define TRIGGER_PIN 2

#define MIN_BAT_VOLTAGE 3000
#define MAX_BAT_VOLTAGE 4120

Atmega328pHal::Atmega328pHal() {
  pinMode(VIBRATOR_PIN, OUTPUT);
  pinMode(LASER_PIN, OUTPUT);
  pinMode(TRIGGER_PIN, INPUT_PULLUP);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

bool Atmega328pHal::triggerIsUp() { return bit_is_set(PIND, PD2); }
bool Atmega328pHal::buttonIsUp() { return bit_is_clear(PIND, PD3); }

/*
 * the 'loop' method shall be called each 10ms
 *
 * Configure timer2 so that an interrupt occurs each 10ms
 * and wake up the MCU if it is in sleep mode.
 */
void Atmega328pHal::setupHeartbeat() {
  TCCR2A = (1U << WGM21) | (0U << WGM20);              // set Timer2 in CTC mode
  TCCR2B = (1U << CS22) | (1U << CS21) | (1U << CS20); // 1/1024 prescaler
  TIMSK2 = (1U << OCIE2A); // enable compare Interrupt
  ASSR &= ~(1U << AS2);
  TCNT2 = 0U;

  // set the output-compare register based on the desired tick frequency
  OCR2A = (F_CPU / BSP_TICKS_PER_SEC / 1024U) - 1U;
}

void Atmega328pHal::laserOn() { PORTB |= (1 << PB2); }
void Atmega328pHal::laserOff() { PORTB &= ~(1 << PB2); }
void Atmega328pHal::vibrationOn() { PORTD |= (1 << PD6); }
void Atmega328pHal::vibrationOff() { PORTD &= ~(1 << PD6); }

uint16_t Atmega328pHal::getBatteryVoltageMv() {

  analogRead(BATTERY_VOLTAGE_PIN);
  float rawBatt = analogRead(BATTERY_VOLTAGE_PIN);
  rawBatt += analogRead(BATTERY_VOLTAGE_PIN);
  rawBatt += analogRead(BATTERY_VOLTAGE_PIN);
  rawBatt += analogRead(BATTERY_VOLTAGE_PIN);
  rawBatt /= 4;
  uint16_t battMv = (5000 / 1023.f) * rawBatt;

  return battMv;
};

uint8_t Atmega328pHal::getBatteryVoltagePercent() {
  return map(getBatteryVoltageMv(), MIN_BAT_VOLTAGE, MAX_BAT_VOLTAGE, 0, 100);
}

bool Atmega328pHal::isCharging() {
  return (digitalRead(CHARGING_STATE_PIN) == HIGH);
}

void Atmega328pHal::sleep() {
  LowPower.idle(SLEEP_15MS, ADC_OFF, TIMER2_ON, TIMER1_OFF, TIMER0_OFF, SPI_OFF,
                USART0_OFF, TWI_OFF);
}

void Atmega328pHal::setGun(Gun *gun) { this->gun = gun; }

extern void buttonInterruptHandler();
extern void triggerInterruptHandler();

void Atmega328pHal::configureInputCallbacks() {
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonInterruptHandler,
                  CHANGE);
  attachInterrupt(digitalPinToInterrupt(TRIGGER_PIN), triggerInterruptHandler,
                  CHANGE);
}