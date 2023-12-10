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

/**
 *
 * This test suite is for targetHost implementation with LDRTarget
 *
 */

#include <ArduinoFake.h>
using namespace fakeit;

#include <Game.hpp>
#include <Target/ITargetUi.hpp>
#include <TargetHost/TargetHost.hpp>

#include <unity.h>

#include "mockTools.hpp"

void tearDown() {}
void setUp() { ArduinoFakeReset(); }

static const uint8_t TRESHOLD_ADDRESS = 0;

void expect_threshold_to_be_storable_in_eeprom() {

  // return LSB then MSB
  When(MOCKED_EEPROM_READ.Using(TRESHOLD_ADDRESS)).Return(20);
  When(MOCKED_EEPROM_UPDATE).AlwaysReturn();

  Mock<ITargetUi> mockGui;
  Game game(&mockGui.get());
  TargetHost app(&game, &mockGui.get());

  app.storeThreshold(20);
  TEST_ASSERT_EQUAL(20, app.getThreshold());

  Verify(MOCKED_EEPROM_UPDATE.Using(TRESHOLD_ADDRESS, 20)).Once();
}

void expect_setup_to_configure_status_led() {
  Mock<ITargetUi> mockGui;
  ITargetUi &ui = mockGui.get();
  Game game(&ui);
  TargetHost app(&game, &ui);
  When(Method(ArduinoFake(), pinMode)).Return();
  When(Method(ArduinoFake(), analogRead)).AlwaysReturn(0);
  When(Method(ArduinoFake(), digitalWrite)).AlwaysReturn();
  When(MOCKED_EEPROM_READ).AlwaysReturn(0);
  When(OverloadedMethod(ArduinoFake(Serial), begin, void(unsigned long)))
      .AlwaysReturn();

  mockGuiForSetup(mockGui);

  app.setup();

  static const uint8_t LED_PIN = 9;
  Verify(Method(ArduinoFake(), pinMode).Using(LED_PIN, OUTPUT)).Once();
}

void expect_ambient_level_to_be_sampled_at_startup_for_each_target() {
  Mock<ITargetUi> mockGui;
  ITargetUi &ui = mockGui.get();
  Game game(&ui);
  TargetHost app(&game, &ui);

  When(Method(ArduinoFake(), pinMode)).Return();
  When(MOCKED_EEPROM_READ).AlwaysReturn(0);
  When(OverloadedMethod(ArduinoFake(Serial), begin, void(unsigned long)))
      .AlwaysReturn();

  When(Method(ArduinoFake(), digitalWrite)).AlwaysReturn();
  When(Method(ArduinoFake(), analogRead)).AlwaysReturn(1000);

  mockGuiForSetup(mockGui);

  app.setup();

  Verify(Method(ArduinoFake(), pinMode)).AtLeastOnce();

  // Note: 5 reads for calibration
  Verify(Method(ArduinoFake(), analogRead).Using(A0)).Exactly(5);
  Verify(Method(ArduinoFake(), analogRead).Using(A1)).Exactly(5);
  Verify(Method(ArduinoFake(), analogRead).Using(A2)).Exactly(5);
  Verify(Method(ArduinoFake(), analogRead).Using(A3)).Exactly(5);
  Verify(Method(ArduinoFake(), analogRead).Using(A4)).Exactly(5);
}

void expect_all_targets_to_be_checked_during_loop() {

  const uint8_t pins[] = {A0, A1, A2, A3, A4};
  const uint8_t LED_PIN = 9;

  for (uint8_t targetIndex = 0; targetIndex < 5; targetIndex++) {

    ArduinoFakeReset();

    Mock<ITargetUi> mockGui;
    ITargetUi &ui = mockGui.get();
    Game game(&ui);
    TargetHost app(&game, &ui);

    // ARRANGE

    // threshold is 100
    When(MOCKED_EEPROM_READ.Using(TRESHOLD_ADDRESS)).Return(100);

    // and target under test luminosity is 150, other target luminosity is 0
    When(Method(ArduinoFake(), analogRead)).AlwaysReturn(0);

    // override only for 'analogRead(pins[targetIndex])'
    When(Method(ArduinoFake(), analogRead).Using(pins[targetIndex]))
        .AlwaysReturn(150);

    // blink
    When(Method(ArduinoFake(), digitalWrite)).AlwaysReturn();
    When(Method(ArduinoFake(), delay)).AlwaysReturn();

    // gui notification
    When(Method(mockGui, hitTarget)).AlwaysReturn();

    // No serial commands
    When(Method(ArduinoFake(Serial), available)).AlwaysReturn(0);

    // ACT
    app.loop();

    // ASSERT
    // luminosity of all target has been read
    Verify(Method(ArduinoFake(), analogRead).Using(pins[targetIndex])).Once();
    Verify(Method(ArduinoFake(), analogRead).Using(Ne(pins[targetIndex])))
        .Exactly(4_Times);
  }
}

void expect_gui_to_be_notified_when_a_target_is_hit() {
  Mock<ITargetUi> mockGui;
  ITargetUi &ui = mockGui.get();
  Game game(&ui);
  TargetHost app(&game, &ui);

  // Run initialization and two loop() call.
  // Ensure GUI is notified *once* when a target is hit

  //
  // setup, provide a value of 10 for threshold
  //
  When(Method(ArduinoFake(), pinMode)).AlwaysReturn();
  When(MOCKED_EEPROM_READ.Using(TRESHOLD_ADDRESS)).AlwaysReturn(10);
  When(OverloadedMethod(ArduinoFake(Serial), begin, void(unsigned long)))
      .AlwaysReturn();

  // Setup GUI mock
  mockGuiForSetup(mockGui);
  
  // No serial commands
  When(Method(ArduinoFake(Serial), available)).AlwaysReturn(0);

  //
  // analog sampling, twice during setup then once during each loops
  //
  When(Method(ArduinoFake(), analogRead)).AlwaysReturn(1000);

  // ambiant at 99, no hit at 100
  When(Method(ArduinoFake(), analogRead).Using(A0))
      .Return(0, 99, 99, 99, 99, 100, 100);

  // ambiant at 108, no hit at 100
  When(Method(ArduinoFake(), analogRead).Using(A1))
      .Return(0, 108, 108, 108, 108, 100, 100);

  // ambiant at 130, *hit*, then 'already hit'
  When(Method(ArduinoFake(), analogRead).Using(A2))
      .Return(0, 130, 130, 130, 130, 141, 141);

  // ambiant at 103, no hit
  When(Method(ArduinoFake(), analogRead).Using(A3))
      .Return(0, 103, 103, 103, 103, 103, 103);

  // ambiant at 96, *hit* then no hit
  When(Method(ArduinoFake(), analogRead).Using(A4))
      .Return(0, 96, 96, 96, 96, 200, 96);

  app.setup();
  app.loop();
  app.loop();

  Verify(Method(ArduinoFake(), pinMode)).AtLeastOnce();
  Verify(Method(ArduinoFake(), analogRead)).Exactly(35);
  Verify(Method(mockGui, hitTarget)).Exactly(2_Times);
}

void expect_threshold_to_be_settable_via_serial_command() {
  Mock<ITargetUi> mockGui;
  ITargetUi &ui = mockGui.get();
  Game game(&ui);
  TargetHost app(&game, &ui);

  mockForCommandTest(mockGui, TRESHOLD_ADDRESS);

  //
  // incoming serial command
  //
  When(Method(ArduinoFake(Serial), available)).Return(5, 4, 3, 2, 1, 0);
  When(Method(ArduinoFake(Serial), read)).Return('T', ' ', '9', '6', '|');

  app.setup();
  app.loop();

  Verify(Method(ArduinoFake(Serial), read)).Exactly(5_Times);

  static const uint8_t expectedThresholdValue = 96;
  TEST_ASSERT_EQUAL(expectedThresholdValue, app.getThreshold());

  // check UI feedback
  Verify(OverloadedMethod(mockGui, log, void(const char *))).Exactly(2_Times);
  Verify(OverloadedMethod(mockGui, log, void(uint8_t))).Exactly(1_Times);
  Verify(MOCKED_EEPROM_UPDATE).Once();
}

void expect_player_to_be_changeable_via_serial_command() {
  Mock<ITargetUi> mockGui;
  Game game(&mockGui.get());
  TargetHost app(&game, &mockGui.get());

  mockForCommandTest(mockGui, TRESHOLD_ADDRESS);

  //
  // incoming serial command
  //
  When(Method(ArduinoFake(Serial), available)).Return(4, 3, 2, 1, 0);
  When(Method(ArduinoFake(Serial), read)).Return('P', ' ', '2', '|');

  app.setup();
  app.loop();

  Verify(Method(ArduinoFake(Serial), read)).Exactly(4_Times);

  static const uint8_t expectedPlayerId = 2;
  TEST_ASSERT_EQUAL(expectedPlayerId, app.game->currentPlayer->id);
}

int main(int, char **) {
  UNITY_BEGIN();
  RUN_TEST(expect_threshold_to_be_storable_in_eeprom);
  RUN_TEST(expect_setup_to_configure_status_led);
  RUN_TEST(expect_ambient_level_to_be_sampled_at_startup_for_each_target);
  RUN_TEST(expect_all_targets_to_be_checked_during_loop);
  RUN_TEST(expect_threshold_to_be_settable_via_serial_command);
  RUN_TEST(expect_player_to_be_changeable_via_serial_command);
  RUN_TEST(expect_gui_to_be_notified_when_a_target_is_hit);

  UNITY_END();
  return 0;
}