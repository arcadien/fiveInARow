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

#include <Game.hpp>
#include <unity.h>
#include <TestGui.hpp>

void tearDown() {}
void setUp() {}



void expect_player_to_be_created_with_initial_values() {
  Player cut(2);
  TEST_ASSERT_EQUAL_UINT8_MESSAGE(2, cut.id, "Player id");
  TEST_ASSERT_EQUAL_UINT8_MESSAGE(0, cut.getTotalHitCount(),
                                  "Player starts with 0 hit shot");
}

void expect_game_to_be_created_with_initial_4_players() {
  TestGui testGui;
  Game cut(&testGui);
  TEST_ASSERT_EQUAL_UINT8_MESSAGE(4, sizeof(cut.players) / sizeof(Player),
                                  "Game player count");
  TEST_ASSERT_NOT_NULL_MESSAGE(cut.currentPlayer,
                               "Current player shall be initialized");
}

void expect_game_to_be_restarted() {
  TestGui testGui;
  Game cut(&testGui);

  cut.recordSucceededShoot();

  TEST_ASSERT_EQUAL_UINT8_MESSAGE(1, cut.currentPlayer->getTotalHitCount(),
                                  "Player hit shot shall be recorded");

  cut.reset();

  TEST_ASSERT_EQUAL_UINT8_MESSAGE(0, cut.currentPlayer->getTotalHitCount(),
                                  "Player info shall be cleared");
}

void expect_game_to_accept_succeeded_shoot_and_update_player_stats() {
  TestGui testGui;
  Game cut(&testGui);

  cut.recordSucceededShoot();

  TEST_ASSERT_EQUAL_UINT8_MESSAGE(1, cut.players[0].hit[0],
                                  "Player shall have one succeeded shoot");
}

void expect_game_to_switch_to_next_player_when_a_round_ends() {
  TestGui testGui;
  Game cut(&testGui);

  TEST_ASSERT_EQUAL_UINT8_MESSAGE(0, cut.currentPlayer->id,
                                  "Very first player shall be player 1");
  cut.nextRound();

  TEST_ASSERT_EQUAL_UINT8_MESSAGE(1, cut.currentPlayer->id,
                                  "Second player shall be player 2");
}

void expect_4_players_to_be_able_to_record_some_hit_shot_in_one_turn() {
  TestGui testGui;
  Game cut(&testGui);

  // player 1 has one hit shot!
  cut.recordSucceededShoot();
  cut.nextRound();

  // player 2 has one hit shot!
  cut.recordSucceededShoot();
  cut.nextRound();

  // player 3 has two hit shot!
  cut.recordSucceededShoot();
  cut.recordSucceededShoot();
  cut.nextRound();

  // player 4 has no hit shot :(
  cut.nextRound();

  TEST_ASSERT_EQUAL_UINT8_MESSAGE(1, cut.players[0].getTotalHitCount(),
                                  "player 1 shall have one hit shot");
  TEST_ASSERT_EQUAL_UINT8_MESSAGE(1, cut.players[1].getTotalHitCount(),
                                  "player 2 shall have one hit shot");
  TEST_ASSERT_EQUAL_UINT8_MESSAGE(2, cut.players[2].getTotalHitCount(),
                                  "player 3 shall have two hit shot");
  TEST_ASSERT_EQUAL_UINT8_MESSAGE(0, cut.players[3].getTotalHitCount(),
                                  "player 4 shall not have any hit shot");
}

void expect_game_to_loop_over_round_until_finished() {
  TestGui testGui;
  Game cut(&testGui);
  static const uint8_t PLAYER_COUNT = 4;
  static const uint8_t ROUND_PER_PLAYER = 5;
  static const uint8_t expectedRounds = PLAYER_COUNT * ROUND_PER_PLAYER;

  uint8_t roundCounter = 0;

  while (!cut.isFinished() && roundCounter <= expectedRounds) {
    cut.nextRound();
    roundCounter++;
  }

  TEST_ASSERT_EQUAL_UINT8_MESSAGE(expectedRounds, roundCounter,
                                  "Maximum round count exceeded");
}

int main(int, char **) {
  UNITY_BEGIN();
  RUN_TEST(expect_player_to_be_created_with_initial_values);
  RUN_TEST(expect_game_to_be_created_with_initial_4_players);
  RUN_TEST(expect_game_to_be_restarted);
  RUN_TEST(expect_game_to_accept_succeeded_shoot_and_update_player_stats);
  RUN_TEST(expect_game_to_switch_to_next_player_when_a_round_ends);
  RUN_TEST(expect_4_players_to_be_able_to_record_some_hit_shot_in_one_turn);
  RUN_TEST(expect_game_to_loop_over_round_until_finished);

  return UNITY_END();
}
