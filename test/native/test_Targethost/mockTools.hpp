#include <ArduinoFake.h>
using namespace fakeit;

#include <Game.hpp>
#include <Target/ITargetUi.hpp>
#include <TargetHost/TargetHost.hpp>

#include <unity.h>

#define MOCKED_EEPROM_READ                                                     \
  OverloadedMethod(ArduinoFake(EEPROM), read, uint8_t(int))

#define MOCKED_EEPROM_UPDATE                                                   \
  OverloadedMethod(ArduinoFake(EEPROM), update, void(int, uint8_t))

void mockGuiForSetup(Mock<ITargetUi> &mockGui);
void mockPrintAndPrinln();
void mockForCommandTest(Mock<ITargetUi> &mockGui);

void mockForCommandTest(Mock<ITargetUi> &mockGui, uint8_t TRESHOLD_ADDRESS) {
  //
  // setup, provide a value of 10 for threshold
  // If serial command interpretation does not work,
  // threshold value at end of test will be 10.
  //
  When(Method(ArduinoFake(), pinMode)).AlwaysReturn();
  When(MOCKED_EEPROM_READ.Using(TRESHOLD_ADDRESS)).AlwaysReturn(10);
  When(MOCKED_EEPROM_UPDATE).AlwaysReturn();

  //
  // blink
  //
  When(Method(ArduinoFake(), digitalWrite)).AlwaysReturn();
  When(Method(ArduinoFake(), delay)).AlwaysReturn();
  When(Method(ArduinoFake(), analogRead)).AlwaysReturn(1000);

  //
  // gui notification
  //
  mockGuiForSetup(mockGui);
  When(Method(mockGui, setCurrentPlayer)).AlwaysReturn();
  When(Method(mockGui, resetTargets)).AlwaysReturn();
  When(Method(mockGui, displayPlayerInfo)).AlwaysReturn();

  When(OverloadedMethod(ArduinoFake(Serial), begin, void(unsigned long)))
      .AlwaysReturn();

  When(Method(ArduinoFake(Serial), end)).AlwaysReturn();
  When(Method(ArduinoFake(Serial), flush)).AlwaysReturn();
  mockPrintAndPrinln();
}

void mockGuiForSetup(Mock<ITargetUi> &mockGui) {
  When(Method(mockGui, restart)).AlwaysReturn();
  When(Method(mockGui, displayPlayerInfo)).AlwaysReturn();
  When(Method(mockGui, setCurrentPlayer)).AlwaysReturn();
  When(Method(mockGui, resetTargets)).AlwaysReturn();
  When(Method(mockGui, hitTarget)).AlwaysReturn();
  When(OverloadedMethod(mockGui, log, void(const char *))).AlwaysReturn();
  When(OverloadedMethod(mockGui, log, void(uint8_t))).AlwaysReturn();
}

void mockPrintAndPrinln() {

  // clang-format off
  When(OverloadedMethod(ArduinoFake(Print), print, size_t(char))).AlwaysReturn();
  When(OverloadedMethod(ArduinoFake(Print), print, size_t(const char *))).AlwaysReturn();
  When(OverloadedMethod(ArduinoFake(Print), print, size_t(unsigned char, int))).AlwaysReturn();
  When(OverloadedMethod(ArduinoFake(Print), print, size_t(int, int))).AlwaysReturn();
  When(OverloadedMethod(ArduinoFake(Print), print, size_t(long, int))).AlwaysReturn();
  When(OverloadedMethod(ArduinoFake(Print), print, size_t(double, int))).AlwaysReturn();
  When(OverloadedMethod(ArduinoFake(Print), print, size_t(unsigned int, int))).AlwaysReturn();
  When(OverloadedMethod(ArduinoFake(Print), print, size_t(unsigned long, int))).AlwaysReturn();

  When(OverloadedMethod(ArduinoFake(Print), println, size_t())).AlwaysReturn();
  When(OverloadedMethod(ArduinoFake(Print), println, size_t(char))).AlwaysReturn();
  When(OverloadedMethod(ArduinoFake(Print), println, size_t(const char *))).AlwaysReturn();
  When(OverloadedMethod(ArduinoFake(Print), println, size_t(unsigned char, int))).AlwaysReturn();
  When(OverloadedMethod(ArduinoFake(Print), println, size_t(int, int))).AlwaysReturn();
  When(OverloadedMethod(ArduinoFake(Print), println, size_t(long, int))).AlwaysReturn();
  When(OverloadedMethod(ArduinoFake(Print), println, size_t(double, int))).AlwaysReturn();
  When(OverloadedMethod(ArduinoFake(Print), println, size_t(unsigned int, int))).AlwaysReturn();
  When(OverloadedMethod(ArduinoFake(Print), println, size_t(unsigned long, int))).AlwaysReturn();

  // clang-format on
}