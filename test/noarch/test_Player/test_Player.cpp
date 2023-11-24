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

#include <Player.hpp>
#include <TestGui.hpp>
#include <unity.h>

void tearDown() {}
void setUp() {}

void expect_player_to_have_5_shots_at_beginning() {
  Player p(0);
  p.startRound();
  TEST_ASSERT_EQUAL_UINT8_MESSAGE(5, p.totalShoots,
                                  "Player first round begins with 5 shoots");
  p.endRound();

  TEST_ASSERT_EQUAL_UINT8_MESSAGE(
      5, p.totalShoots, "Shoots are only incremented when round starts");
}

void expect_met_count_to_spread_well_on_five_rounds() {
  Player p(0);

  // round 0
  p.startRound();
  p.recordSucceededShoot();
  p.endRound();

  // round 1
  p.startRound();
  p.recordSucceededShoot();
  p.endRound();

  // round 2
  p.startRound();
  p.recordSucceededShoot();
  p.endRound();

  // round 3
  p.startRound();
  p.recordSucceededShoot();
  p.endRound();

  // round 4
  p.startRound();
  p.recordSucceededShoot();
  p.endRound();

  TEST_ASSERT_EQUAL_UINT8_MESSAGE(
      5, p.getTotalHitCount(), "Met shots shall be collected from all rounds");
}

void expect_a_player_to_stop_counting_hit_after_5_in_a_round() {
  Player p(0);
  p.startRound();
  p.recordSucceededShoot();
  p.recordSucceededShoot();
  p.recordSucceededShoot();
  p.recordSucceededShoot();
  p.recordSucceededShoot();
  // over round
  p.recordSucceededShoot();
  p.recordSucceededShoot();
  p.recordSucceededShoot();
  p.recordSucceededShoot();

  TEST_ASSERT_EQUAL_UINT8_MESSAGE(5, p.getTotalHitCount(),
                                  "Hit shots shall not exceed 5 in a round");

  p.endRound();
  p.recordSucceededShoot();

  TEST_ASSERT_EQUAL_UINT8_MESSAGE(
      6, p.getTotalHitCount(),
      "After switching to a new round, hit count resumes");
}

void expect_state_of_player_to_be_clear_after_a_reset() {
  Player p(0);
  p.startRound();
  p.recordSucceededShoot();
  p.startRound();
  p.recordSucceededShoot();

  TEST_ASSERT_EQUAL_UINT8_MESSAGE(2, p.getTotalHitCount(),
                                  "Hit count shall be incremented");

  TEST_ASSERT_EQUAL_UINT8_MESSAGE(
      10, p.totalShoots, "After two rounds, there shall be 10 total shoots");

  p.reset();

  TEST_ASSERT_EQUAL_UINT8_MESSAGE(0, p.getTotalHitCount(),
                                  "After reset, there shall be no hit");

  TEST_ASSERT_EQUAL_UINT8_MESSAGE(0, p.totalShoots,
                                  "After reset, there shall be no shoot");
}
int main(int, char **) {
  UNITY_BEGIN();
  RUN_TEST(expect_player_to_have_5_shots_at_beginning);
  RUN_TEST(expect_met_count_to_spread_well_on_five_rounds);
  RUN_TEST(expect_a_player_to_stop_counting_hit_after_5_in_a_round);
  RUN_TEST(expect_state_of_player_to_be_clear_after_a_reset);

  return UNITY_END();
}
