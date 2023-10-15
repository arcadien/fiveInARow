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
  TEST_ASSERT_EQUAL_UINT8_MESSAGE(5, p.totalShoots,
                                  "Player begins with 5 shoots");
  p.nextRound();
  TEST_ASSERT_EQUAL_UINT8_MESSAGE(10, p.totalShoots,
                                  "Shoots are incremented when round ends");
}
void expect_met_count_to_spread_well_on_five_rounds() {
  Player p(0);

  // round 0
  p.recordSucceededShoot();
  p.nextRound();

  // round 1
  p.recordSucceededShoot();
  p.nextRound();

  // round 2
  p.recordSucceededShoot();
  p.nextRound();

  // round 3
  p.recordSucceededShoot();
  p.nextRound();

  // round 4
  p.recordSucceededShoot();

  TEST_ASSERT_EQUAL_UINT8_MESSAGE(
      5, p.getTotalHitCount(), "Met shots shall be collected from all rounds");
}

void expect_a_player_to_stop_counting_hit_after_5_in_a_round() {
  Player p(0);
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

  TEST_ASSERT_EQUAL_UINT8_MESSAGE(
      5, p.getTotalHitCount(), "Hit shots shall not exceed 5 in a round");
  
  p.nextRound();
  p.recordSucceededShoot();

    TEST_ASSERT_EQUAL_UINT8_MESSAGE(
      6, p.getTotalHitCount(), "After switching to a new round, hit count resumes");
}

int main(int, char **) {
  UNITY_BEGIN();
  RUN_TEST(expect_player_to_have_5_shots_at_beginning);
  RUN_TEST(expect_met_count_to_spread_well_on_five_rounds);
  RUN_TEST(expect_a_player_to_stop_counting_hit_after_5_in_a_round);

  return UNITY_END();
}
