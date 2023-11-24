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

#include <Contactor/Contactor.hpp>
#include <fakeit.hpp>
#include <unity.h>

using namespace fakeit;

Contactor obj;
Mock<Contactor> spy(obj);

void tearDown() { spy.Reset(); }

void setUp() {}

void expect_short_press_to_be_evaluated_on_contactor_up() {

  Spy(Method(spy, onShortPress));
  Contactor &sut = spy.get();

  long now = 1;

  sut.onDown(now);
  sut.onUp(now);

  Verify(Method(spy, onShortPress)).Once();
}

void expect_long_press_to_be_evaluated_on_contactor_up() {

  Spy(Method(spy, onShortPress), Method(spy, onLongPress),
      Method(spy, checkForLongPress));
  Contactor &sut = spy.get();

  long now = 1;

  sut.onDown(now);
  sut.onUp(now + 2002);

  Verify(Method(spy, checkForLongPress)).Once();
  Verify(Method(spy, onLongPress)).Once();
}

void expect_long_press_to_not_be_called_on_up_in_a_too_short_time() {

  Spy(Method(spy, onShortPress), Method(spy, onLongPress),
      Method(spy, checkForLongPress));
  Contactor &sut = spy.get();

  long now = 1;

  sut.onDown(now);
  sut.onUp(now + 100);

  Verify(Method(spy, onShortPress)).Once();
  Verify(Method(spy, checkForLongPress)).Once();
  Verify(Method(spy, onLongPress)).Exactly(0);
}

void expect_long_press_to_be_called_on_up_in_a_sufficient_time() {

  Spy(Method(spy, onShortPress), Method(spy, onLongPress),
      Method(spy, checkForLongPress));
  Contactor &sut = spy.get();

  long now = 1;

  sut.onDown(now);
  sut.onUp(now + 2500);

  Verify(Method(spy, checkForLongPress)).Once();
  Verify(Method(spy, onLongPress)).Once();
}

void expect_long_press_to_not_trigger_short_press_on_release() {

  Spy(Method(spy, onShortPress), Method(spy, onLongPress),
      Method(spy, checkForLongPress));
  Contactor &sut = spy.get();

  long now = 1;

  sut.onDown(now);
  sut.onUp(now + 2500);

  Verify(Method(spy, onShortPress)).Exactly(0);
}

void expect_long_press_to_not_evaluate_if_button_is_not_down() {

  Spy(Method(spy, onShortPress), Method(spy, onLongPress));
  Contactor &sut = spy.get();

  long now = 1;

  sut.onDown(now);
  sut.onUp(now + 100);

  sut.checkForLongPress(now + 3000);

  Verify(Method(spy, onShortPress)).Once();
  Verify(Method(spy, onLongPress)).Exactly(0);
}

void expect_long_press_to_be_evaluatable_while_contactor_is_down() {

  Spy(Method(spy, onShortPress), Method(spy, onLongPress));
  Contactor &sut = spy.get();

  long now = 1;

  sut.onDown(now);
  sut.checkForLongPress(now + 2002);

  Verify(Method(spy, onShortPress)).Exactly(0);
  Verify(Method(spy, onLongPress)).Once();
}



int main(int, char **) {
  UNITY_BEGIN();
  RUN_TEST(expect_short_press_to_be_evaluated_on_contactor_up);
  RUN_TEST(expect_long_press_to_be_evaluated_on_contactor_up);
  RUN_TEST(expect_long_press_to_not_be_called_on_up_in_a_too_short_time);
  RUN_TEST(expect_long_press_to_be_called_on_up_in_a_sufficient_time);
  RUN_TEST(expect_long_press_to_not_trigger_short_press_on_release);
  RUN_TEST(expect_long_press_to_not_evaluate_if_button_is_not_down);
  RUN_TEST(expect_long_press_to_be_evaluatable_while_contactor_is_down);

  return UNITY_END();
}
