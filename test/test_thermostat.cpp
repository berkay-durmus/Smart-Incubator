#include <cstdio>
#include <cstdlib>

#include "thermostat.h"

using incubator::Limits;
using incubator::State;
using incubator::Thermostat;

static int failures = 0;

#define CHECK(condition)                                                  \
  do {                                                                    \
    if (!(condition)) {                                                   \
      std::printf("FAIL %s:%d  %s\n", __FILE__, __LINE__, #condition);    \
      ++failures;                                                         \
    }                                                                     \
  } while (0)

static Thermostat make() { return Thermostat(Limits{36.0f, 39.0f, 0.5f}); }

static void test_band_boundaries() {
  CHECK(make().update(35.9f, true) == State::Heating);
  CHECK(make().update(36.0f, true) == State::Idle);
  CHECK(make().update(38.9f, true) == State::Idle);
  CHECK(make().update(39.0f, true) == State::Cooling);
}

static void test_heating_hysteresis() {
  Thermostat t = make();
  CHECK(t.update(35.0f, true) == State::Heating);
  CHECK(t.update(36.4f, true) == State::Heating);
  CHECK(t.update(36.5f, true) == State::Idle);
  CHECK(t.update(35.9f, true) == State::Heating);
}

static void test_cooling_hysteresis() {
  Thermostat t = make();
  CHECK(t.update(40.0f, true) == State::Cooling);
  CHECK(t.update(38.6f, true) == State::Cooling);
  CHECK(t.update(38.5f, true) == State::Idle);
  CHECK(t.update(39.0f, true) == State::Cooling);
}

static void test_direct_swing_between_heating_and_cooling() {
  Thermostat t = make();
  CHECK(t.update(30.0f, true) == State::Heating);
  CHECK(t.update(41.0f, true) == State::Cooling);
  CHECK(t.update(30.0f, true) == State::Heating);
}

static void test_invalid_reading_switches_everything_off_and_recovers() {
  Thermostat t = make();
  t.update(30.0f, true);
  State s = t.update(0.0f, false);
  CHECK(s == State::Fault);
  CHECK(!incubator::heater_on(s) && !incubator::cooler_on(s));
  CHECK(t.update(37.0f, true) == State::Idle);
}

static void test_outputs_are_never_both_on() {
  Thermostat t = make();
  for (float temp = 25.0f; temp <= 45.0f; temp += 0.25f) {
    State s = t.update(temp, true);
    CHECK(!(incubator::heater_on(s) && incubator::cooler_on(s)));
  }
  for (float temp = 45.0f; temp >= 25.0f; temp -= 0.25f) {
    State s = t.update(temp, true);
    CHECK(!(incubator::heater_on(s) && incubator::cooler_on(s)));
  }
}

int main() {
  test_band_boundaries();
  test_heating_hysteresis();
  test_cooling_hysteresis();
  test_direct_swing_between_heating_and_cooling();
  test_invalid_reading_switches_everything_off_and_recovers();
  test_outputs_are_never_both_on();

  if (failures) {
    std::printf("%d check(s) failed\n", failures);
    return EXIT_FAILURE;
  }
  std::printf("all tests passed\n");
  return EXIT_SUCCESS;
}
