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

#include <Adafruit_SSD1306.h>
#include <Fonts/FreeMonoBold18pt7b.h>
#include <Fonts/FreeMonoOblique9pt7b.h>

#include <LowPower.h>

#define TRIGGER_PIN 2
#define BUTTON2_PIN 3

#define LASER_PIN 10
#define OUT2_PIN 6
#define BATTERY_VOLTAGE_PIN A3
#define CHARGING_STATE_PIN A2

#define SHOOT_DURATION_CYCLE 5
#define MIN_BAT_VOLTAGE 3000
#define MAX_BAT_VOLTAGE 4120

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET -1    // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS                                                         \
  0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

volatile uint16_t hitcount;
uint16_t previoushitcout;
volatile uint16_t shootCount;

#define BSP_TICKS_PER_SEC 100
#define F_CPU 16000000L
#define TICKS_BETWEEN_UI_UPDATE 1000 // 10 sec

// counter for ticks while firing
volatile uint8_t shootCycleCountdown;

// counter for ticks between battery display update
volatile uint16_t updateBatteryDisplayCycleCount;

void shoot() {
  // wake up!
  if (shootCycleCountdown == 0) {
    shootCount += 1;
    digitalWrite(LASER_PIN, HIGH);
    digitalWrite(OUT2_PIN, HIGH);
    shootCycleCountdown = SHOOT_DURATION_CYCLE;
  }
}
void reload() {
  // wake up!
  shootCount = 0;
}

void setupHeartbeat() {

  // set Timer2 in CTC mode, 1/1024 prescaler, start the timer ticking...
  TCCR2A = (1U << WGM21) | (0U << WGM20);
  TCCR2B = (1U << CS22) | (1U << CS21) | (1U << CS20); // 1/2^10
  ASSR &= ~(1U << AS2);
  TIMSK2 = (1U << OCIE2A); // enable TIMER2 compare Interrupt
  TCNT2 = 0U;

  // set the output-compare register based on the desired tick frequency
  OCR2A = (F_CPU / BSP_TICKS_PER_SEC / 1024U) - 1U;
}

ISR(TIMER2_COMPA_vect) {
  if (shootCycleCountdown > 0) {
    shootCycleCountdown--;
  } else if (updateBatteryDisplayCycleCount > 0) {
    updateBatteryDisplayCycleCount--;
  }
}

static void displayShootCount() {
  display.fillRect(11, 0, 68, 30, 0); // clear

  if (shootCount > 999)
    shootCount = 0;

  if (shootCount < 10) {
    display.setCursor(15, 25);
  } else {
    display.setCursor(10, 25);
  }
  display.print(shootCount);
  display.display();
}

static void displayBatteryStatus() {

  // schedule next update
  updateBatteryDisplayCycleCount = TICKS_BETWEEN_UI_UPDATE;

  analogRead(BATTERY_VOLTAGE_PIN);
  float rawBatt = analogRead(BATTERY_VOLTAGE_PIN);
  rawBatt += analogRead(BATTERY_VOLTAGE_PIN);
  rawBatt += analogRead(BATTERY_VOLTAGE_PIN);
  rawBatt += analogRead(BATTERY_VOLTAGE_PIN);
  rawBatt /= 4;

  // https://rickkas7.github.io/DisplayGenerator/index.html

  float battMv = (5000 / 1023.f) * rawBatt;
  bool isCharging = (digitalRead(CHARGING_STATE_PIN) == HIGH);

#if defined(SERIAL_OUTPUT)
  Serial.print(rawBatt);
  Serial.print(';');
  Serial.print(battMv);
  Serial.print(';');
  Serial.println(isCharging ? "c" : "n");
  delay(2);
#endif

  uint8_t battPercent = map(battMv, MIN_BAT_VOLTAGE, MAX_BAT_VOLTAGE, 0, 100);

  display.setFont(NULL);
  display.fillRect(79, 0, 59, 24, 0); // clear
  display.setCursor(100, 2);
  display.print(battPercent);
  display.println("%");
  if (isCharging) {
    display.setCursor(80, 15);
    display.println("charging");
  }

  // prepare font for next shoot count display
  display.setFont(&FreeMonoBold18pt7b);
}

void setup(void) {

#if defined(SERIAL_OUTPUT)
  Serial.begin(115200);
  while (!Serial) {
  }
#endif
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
#if defined(SERIAL_OUTPUT)
    Serial.println(F("SSD1306 allocation failed"));
#endif
    for (;;)
      ; // Don't proceed, loop forever
  }
  shootCount = 0;
  shootCycleCountdown = 0;
  updateBatteryDisplayCycleCount = TICKS_BETWEEN_UI_UPDATE;

  pinMode(LASER_PIN, OUTPUT);
  digitalWrite(LASER_PIN, LOW);

  pinMode(OUT2_PIN, OUTPUT);
  digitalWrite(OUT2_PIN, LOW);

  pinMode(BATTERY_VOLTAGE_PIN, INPUT);
  pinMode(CHARGING_STATE_PIN, INPUT);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

#if defined(SERIAL_OUTPUT)
  Serial.println("Ready!");
#endif

  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setFont(&FreeMonoOblique9pt7b);
  display.setCursor(9, 11);
  display.println("5 in a row");
  display.setCursor(4, 27);
  display.println(" # # # # #");
  display.display();

  delay(2000);

  display.clearDisplay();
  displayBatteryStatus();
  displayShootCount();

  pinMode(BUTTON2_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON2_PIN), reload, FALLING);

  pinMode(TRIGGER_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(TRIGGER_PIN), shoot, FALLING);
  cli();
  setupHeartbeat();
  sei();
}

void loop(void) {

  if (shootCycleCountdown == 0) {

    digitalWrite(LASER_PIN, LOW);
    digitalWrite(OUT2_PIN, LOW);
    displayShootCount();

    if (updateBatteryDisplayCycleCount == 0) {
      displayBatteryStatus();
    }
  }
  LowPower.idle(SLEEP_FOREVER, ADC_OFF, TIMER2_ON, TIMER1_OFF, TIMER0_OFF,
                SPI_OFF, USART0_OFF, TWI_OFF);
}