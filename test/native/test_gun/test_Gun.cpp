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
#include <Gun/IGunHal.hpp>
#include <Gun/IGunUi.hpp>

#include <unity.h>

#include <ArduinoFake.h>
using namespace fakeit;

#define MOCK_ALL()                                                             \
  When(Method(mockHal, triggerIsUp)).AlwaysReturn(true);                       \
  When(Method(mockHal, buttonIsUp)).AlwaysReturn(true);                        \
  When(Method(mockHal, laserOn)).AlwaysReturn();                               \
  When(Method(mockHal, vibrationOn)).AlwaysReturn();                           \
  When(Method(mockHal, vibrationOff)).AlwaysReturn();                          \
  When(Method(mockHal, laserOff)).AlwaysReturn();                              \
  When(Method(mockHal, getBatteryVoltageMv)).AlwaysReturn(5000U);              \
  When(Method(mockHal, getBatteryVoltagePercent)).AlwaysReturn(100U);          \
  When(Method(mockHal, sleep)).AlwaysReturn();                                 \
  When(Method(mockUi, displayBatteryStatus)).AlwaysReturn();                   \
  When(Method(mockUi, displayShootCount)).AlwaysReturn();                      \
  When(Method(mockUi, displayCalibration)).AlwaysReturn();                     \
  When(Method(mockUi, clearCalibration)).AlwaysReturn();                       \
  When(Method(mockUi, displaySplash)).AlwaysReturn();

void tearDown() {}
void setUp() { ArduinoFakeReset(); }

void expect_gun_to_loop() {

  Mock<IGunUi> mockUi;
  Mock<IGunHal> mockHal;

  MOCK_ALL();

  IGunUi &ui = mockUi.get();
  IGunHal &hal = mockHal.get();

  Gun gun(&hal, &ui);

  gun.loop();
}
void expect_gun_to_shoot_50ms_on_trigger_down() {

  Mock<IGunUi> mockUi;
  Mock<IGunHal> mockHal;

  MOCK_ALL();

  IGunUi &ui = mockUi.get();
  IGunHal &hal = mockHal.get();

  Gun gun(&hal, &ui);

  gun.trigger.pendingEvent = Contactor::Event::Pressed;
  gun.loop();
  gun.loop();
  gun.trigger.pendingEvent = Contactor::Event::Released;
  gun.loop();
  gun.trigger.pendingEvent = Contactor::Event::NoEvent;
  gun.loop();
  gun.loop();
  gun.loop();
  gun.loop();

  // trigger is pressed then released,
  // activating laser for 50ms/5 ticks
  Verify(Method(mockHal, laserOn)).Once();
  Verify(Method(mockHal, vibrationOn)).Once();
  Verify(Method(mockHal, vibrationOff)).Once();
  Verify(Method(mockHal, laserOff)).Once();
}

void expect_ui_to_display_battery_state_at_boot_and_each_1s() {
  Mock<IGunUi> mockUi;
  Mock<IGunHal> mockHal;

  MOCK_ALL();

  IGunUi &ui = mockUi.get();
  IGunHal &hal = mockHal.get();

  Gun gun(&hal, &ui);

  for (uint8_t tickCounter = 0; tickCounter <= 101; tickCounter++) {
    // displayBatteryStatus call after 100 ticks
    gun.loop();
  }

  // display at first loop, then each second (or 100 ticks)
  Verify(Method(mockUi, displayBatteryStatus)).Exactly(2);
}

void expect_switch_to_maintenance_after_2s_button_continuous_press() {
  Mock<IGunUi> mockUi;
  Mock<IGunHal> mockHal;

  MOCK_ALL();

  IGunUi &ui = mockUi.get();
  IGunHal &hal = mockHal.get();

  Gun gun(&hal, &ui);

  gun.button.pendingEvent = Contactor::Event::Pressed;
  for (uint8_t tickCounter = 0; tickCounter <= 200; tickCounter++) {
    // long press shall be accounted after 2s, 200 ticks
    gun.loop();
  }
  gun.button.pendingEvent = Contactor::Event::Released;
  gun.loop();
  gun.button.pendingEvent = Contactor::Event::Pressed;
  for (uint8_t tickCounter = 0; tickCounter <= 200; tickCounter++) {
    // long press shall be accounted after 2s, 200 ticks
    gun.loop();
  }

  // switch to calibration
  Verify(Method(mockHal, laserOn)).Exactly(1);
  Verify(Method(mockUi, displayCalibration)).Exactly(1);

  // back to normal
  Verify(Method(mockHal, laserOff)).Exactly(1);
  Verify(Method(mockUi, clearCalibration)).Exactly(1);
  Verify(Method(mockUi, displayShootCount)).Exactly(1);
}

void expect_button_press_to_reset_shoot_count_and_redisplay() {
  Mock<IGunUi> mockUi;
  Mock<IGunHal> mockHal;
  MOCK_ALL();
  IGunUi &ui = mockUi.get();
  IGunHal &hal = mockHal.get();

  Gun gun(&hal, &ui);

  gun.shootCount = 10;

  gun.button.pendingEvent = Contactor::Event::Pressed;
  gun.loop();
  gun.button.pendingEvent = Contactor::Event::Released;
  gun.loop();

  TEST_ASSERT_EQUAL_MESSAGE(0, gun.shootCount, "Shoot count shall be reset");
  Verify(Method(mockUi, displayShootCount)).Exactly(1);
}

void expect_trigger_to_have_no_effect_in_calibration_mode() {
  Mock<IGunUi> mockUi;
  Mock<IGunHal> mockHal;

  MOCK_ALL();

  IGunUi &ui = mockUi.get();
  IGunHal &hal = mockHal.get();

  Gun gun(&hal, &ui);

  gun.button.pendingEvent = Contactor::Event::Pressed;
  for (uint8_t tickCounter = 0; tickCounter <= 200; tickCounter++) {
    // long press shall be accounted after 2s, 200 ticks
    gun.loop();
  }

  gun.button.pendingEvent = Contactor::Event::Released;
  gun.loop();

  gun.trigger.pendingEvent = Contactor::Event::Pressed;
  gun.loop();
  gun.trigger.pendingEvent = Contactor::Event::Released;
  gun.loop();

  Verify(Method(mockHal, laserOn)).Exactly(1);
  Verify(Method(mockHal, laserOff)).Exactly(0);
}

void expect_long_press_to_trigger_only_if_button_is_down() {
  Mock<IGunUi> mockUi;
  Mock<IGunHal> mockHal;

  MOCK_ALL();

  IGunUi &ui = mockUi.get();
  IGunHal &hal = mockHal.get();

  Gun gun(&hal, &ui);

  gun.button.pendingEvent = Contactor::Event::Pressed;
  gun.loop();

  gun.button.pendingEvent = Contactor::Event::Released;
  gun.loop();

  for (uint8_t tickCounter = 0; tickCounter <= 200; tickCounter++) {
    // long press shall be accounted after 2s, 200 ticks
    gun.loop();
  }

  // No long press, no switch to calibration
  Verify(Method(mockHal, laserOn)).Exactly(0);
  Verify(Method(mockUi, displayCalibration)).Exactly(0);
  Verify(Method(mockHal, laserOff)).Exactly(0);
  Verify(Method(mockUi, clearCalibration)).Exactly(0);
  Verify(Method(mockUi, displayShootCount)).Exactly(1);
}

int main(int, char **) {
  UNITY_BEGIN();
  RUN_TEST(expect_gun_to_loop);
  RUN_TEST(expect_gun_to_shoot_50ms_on_trigger_down);
  RUN_TEST(expect_ui_to_display_battery_state_at_boot_and_each_1s);
  RUN_TEST(expect_switch_to_maintenance_after_2s_button_continuous_press);
  RUN_TEST(expect_long_press_to_trigger_only_if_button_is_down);
  RUN_TEST(expect_button_press_to_reset_shoot_count_and_redisplay);
  RUN_TEST(expect_trigger_to_have_no_effect_in_calibration_mode);

  UNITY_END();
  return 0;
}
