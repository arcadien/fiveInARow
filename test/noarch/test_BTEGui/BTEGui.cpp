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

#include <BTEGui.hpp>
#include <exception>
#include <iostream>
#include <sstream>
#include <unity.h>

void tearDown() {}
void setUp() {}

void expect_target_to_manage_5_targets() {
  std::stringstream fakeSerial;
  BTEGui cut(fakeSerial);
  std::string actual;

  cut.hitTarget(ITargetGui::TARGET::One);

  std::string expectedLine = "*AR255G255B255*";
  std::getline(fakeSerial, actual);
  TEST_ASSERT_EQUAL_STRING_MESSAGE(expectedLine.c_str(), actual.c_str(),
                                   "Hit target shall be white");

  cut.hitTarget(ITargetGui::TARGET::Two);

  expectedLine = "*ZR255G255B255*";
  std::getline(fakeSerial, actual);
  TEST_ASSERT_EQUAL_STRING_MESSAGE(expectedLine.c_str(), actual.c_str(),
                                   "Hit target shall be white");

  cut.hitTarget(ITargetGui::TARGET::Three);

  expectedLine = "*ER255G255B255*";
  std::getline(fakeSerial, actual);
  TEST_ASSERT_EQUAL_STRING_MESSAGE(expectedLine.c_str(), actual.c_str(),
                                   "Hit target shall be white");

  cut.hitTarget(ITargetGui::TARGET::Four);

  expectedLine = "*RR255G255B255*";
  std::getline(fakeSerial, actual);
  TEST_ASSERT_EQUAL_STRING_MESSAGE(expectedLine.c_str(), actual.c_str(),
                                   "Hit target shall be white");

  cut.hitTarget(ITargetGui::TARGET::Five);

  expectedLine = "*TR255G255B255*";
  std::getline(fakeSerial, actual);
  TEST_ASSERT_EQUAL_STRING_MESSAGE(expectedLine.c_str(), actual.c_str(),
                                   "Hit target shall be white");

  TEST_ASSERT_TRUE(cut.isTargetHit(ITargetGui::TARGET::One));
  TEST_ASSERT_TRUE(cut.isTargetHit(ITargetGui::TARGET::Two));
  TEST_ASSERT_TRUE(cut.isTargetHit(ITargetGui::TARGET::Three));
  TEST_ASSERT_TRUE(cut.isTargetHit(ITargetGui::TARGET::Four));
  TEST_ASSERT_TRUE(cut.isTargetHit(ITargetGui::TARGET::Five));
}

void expect_gui_to_display_player_info() {
  std::stringstream fakeSerial;
  BTEGui cut(fakeSerial);

  /**
   * Structure used to build test
   */
  struct TestData {
    Player givenPlayer;
    uint8_t givenPlayerId;
    uint8_t givenRound;
    uint8_t givenHit;
    const char *expectedLine1;
    const char *expectedLine2;
  };

  // clang-format off
  TestData testCases[4] = {
    {Player(0), givenPlayerId: 0, givenRound: 1, givenHit: 1, "*Q5*",  "*W1*"},
    {Player(1), givenPlayerId: 1, givenRound: 2, givenHit: 3, "*S10*", "*X3*"},
    {Player(2), givenPlayerId: 2, givenRound: 3, givenHit: 4, "*D15*", "*C4*"},
    {Player(3), givenPlayerId: 3, givenRound: 2, givenHit: 5, "*F10*", "*V5*"} 
  };
  // clang-format on
  std::stringstream buffer;
  for (auto testCase : testCases) {
    buffer.clear();
    Player *currentPlayer = &testCase.givenPlayer;

    TEST_ASSERT_EQUAL_UINT8(currentPlayer->id, testCase.givenPlayerId);

    for (uint8_t round = 0; round < testCase.givenRound; round++) {
      currentPlayer->startRound();
      currentPlayer->endRound();
    }

    // note: for simplicity sake, all hits are recorded in the last round
    for (uint8_t hit = 0; hit < testCase.givenHit; hit++) {
      currentPlayer->recordSucceededShoot();
    }
    cut.displayPlayerInfo(*currentPlayer);

    std::string actual;
    std::getline(fakeSerial, actual);

    buffer << "( Player ";
    buffer << unsigned(testCase.givenPlayer.id) << "  total shoots)"
           << std::endl;
    std::string message;
    std::getline(buffer, message);

    TEST_ASSERT_EQUAL_STRING_MESSAGE(testCase.expectedLine1, actual.c_str(),
                                     message.c_str());

    buffer.clear();
    buffer << "( Player ";
    buffer << unsigned(testCase.givenPlayer.id) << " hit shoots)" << std::endl;
    std::getline(buffer, message);

    std::getline(fakeSerial, actual);
    TEST_ASSERT_EQUAL_STRING_MESSAGE(testCase.expectedLine2, actual.c_str(),
                                     message.c_str());
  }
}

int main(int, char **) {
  UNITY_BEGIN();
  RUN_TEST(expect_target_to_manage_5_targets);
  RUN_TEST(expect_gui_to_display_player_info);

  return UNITY_END();
}
