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

#include <Gun.hpp>
#include <TestGui.hpp>
#include <unity.h>

#include <ArduinoFake.h>
using namespace fakeit;

#define MOCK_ALL()                                                             \
  When(Method(mock, vibrationOn)).AlwaysReturn();                              \
  When(Method(mock, vibrationOff)).AlwaysReturn();                             \
  When(Method(mock, laserOn)).AlwaysReturn();                                  \
  When(Method(mock, laserOff)).AlwaysReturn();                                 \
  When(Method(mock, ledOn)).AlwaysReturn();                                    \
  When(Method(mock, ledOff)).AlwaysReturn();                                   \
  When(Method(mock, shortDelay)).AlwaysReturn();                               \
  When(Method(mock, longDelay)).AlwaysReturn();                                \
  When(Method(mock, isButton1Pressed)).AlwaysReturn();                         \
  When(Method(mock, isButton2Pressed)).AlwaysReturn();                         \
  When(Method(mock, deepSleep)).AlwaysReturn();

void tearDown() {}
void setUp() { ArduinoFakeReset(); }

void expect_shot_to_activate_laser_during_short_duration() {

  Mock<IGunHal> mock;
  MOCK_ALL();
  IGunHal &hal = mock.get();
  Gun gun(hal);

  // first, give 5 shots
  gun.onButton2ShortPress();

  // shot
  gun.onButton1ShortPress();

  Verify(Method(mock, laserOn)).Once();
  Verify(Method(mock, shortDelay)).Once();
  Verify(Method(mock, laserOff)).Once();
}

void expect_shot_to_activate_vibration_during_short_duration() {

  Mock<IGunHal> mock;
  MOCK_ALL();
  IGunHal &hal = mock.get();
  Gun gun(hal);

  // first, give 5 shots
  gun.onButton2ShortPress();

  // shot
  gun.onButton1ShortPress();

  Verify(Method(mock, vibrationOn)).Once();
  Verify(Method(mock, shortDelay)).Once();
  Verify(Method(mock, vibrationOff)).Once();
}

void expect_short_press_on_button2_to_give_5_shots() {
  Mock<IGunHal> mock;
  MOCK_ALL();
  IGunHal &hal = mock.get();
  Gun gun(hal);
  gun.onButton2ShortPress();
  TEST_ASSERT_EQUAL_INT_MESSAGE(5, gun.availableShots,
                                "Shots shall have been incremented");

  gun.onButton2ShortPress();
  TEST_ASSERT_EQUAL_INT_MESSAGE(5, gun.availableShots,
                                "A rearm always provide 5 shots");
}

void expect_shot_to_decrement_available_shot_count() {
  Mock<IGunHal> mock;
  MOCK_ALL();
  IGunHal &hal = mock.get();
  Gun gun(hal);

  // first, give 5 shots
  gun.onButton2ShortPress();

  gun.onButton1ShortPress();
  TEST_ASSERT_EQUAL_INT_MESSAGE(
      4, gun.availableShots,
      "There shall be 4 more available shots after 1 shots");
  gun.onButton1ShortPress();
  gun.onButton1ShortPress();
  gun.onButton1ShortPress();
  gun.onButton1ShortPress();
  TEST_ASSERT_EQUAL_INT_MESSAGE(
      0, gun.availableShots,
      "There shall be no more available shots after 5 shots");
  gun.onButton1ShortPress();

  // no bug when shooting with a remaining of 0
  TEST_ASSERT_EQUAL_INT_MESSAGE(
      0, gun.availableShots,
      "There shall be no more available shots after 5 shots");
}

void expect_shot_to_do_nothing_if_there_is_no_available_shots() {
  Mock<IGunHal> mock;
  MOCK_ALL();
  IGunHal &hal = mock.get();
  Gun gun(hal);

  // first, give 5 shots
  gun.onButton2ShortPress();

  gun.onButton1ShortPress(); // shot 1
  gun.onButton1ShortPress(); // shot 2
  gun.onButton1ShortPress(); // shot 3
  gun.onButton1ShortPress(); // shot 4
  gun.onButton1ShortPress(); // shot 5
  gun.onButton1ShortPress(); // shot 6, which shall do nothing

  Verify(Method(mock, vibrationOn)).Exactly(5);
  Verify(Method(mock, vibrationOff)).Exactly(5);
  Verify(Method(mock, laserOn)).Exactly(5);
  Verify(Method(mock, laserOff)).Exactly(5);
  Verify(Method(mock, shortDelay)).Exactly(5);
  Verify(Method(mock, vibrationOff)).Exactly(5);
}

void expect_long_press_on_button_two_to_activate_continuous_laser() {
  Mock<IGunHal> mock;
  MOCK_ALL();
  IGunHal &hal = mock.get();
  Gun gun(hal);

  gun.onButton2LongPress();

  Verify(Method(mock, laserOn)).Once();
  Verify(Method(mock, shortDelay)).Exactly(0);
  Verify(Method(mock, laserOff)).Exactly(0);
}

void expect_long_press_on_button_one_to_trigger_deep_sleep()
{
  Mock<IGunHal> mock;
  When(Method(mock, deepSleep)).Return();
  IGunHal &hal = mock.get();
  Gun gun(hal);

  gun.onButton1LongPress();

  Verify(Method(mock, deepSleep)).Once();
}

int main(int, char **) {
  UNITY_BEGIN();
  RUN_TEST(expect_short_press_on_button2_to_give_5_shots);
  RUN_TEST(expect_shot_to_activate_laser_during_short_duration);
  RUN_TEST(expect_shot_to_activate_vibration_during_short_duration);
  RUN_TEST(expect_shot_to_decrement_available_shot_count);
  RUN_TEST(expect_shot_to_do_nothing_if_there_is_no_available_shots);
  RUN_TEST(expect_long_press_on_button_two_to_activate_continuous_laser);
  RUN_TEST(expect_long_press_on_button_one_to_trigger_deep_sleep);

  UNITY_END();
  return 0;
}
