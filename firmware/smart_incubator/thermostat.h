#pragma once

// Hardware-independent control logic, so it can be unit-tested on a PC.

namespace incubator {

enum class State : unsigned char { Idle, Heating, Cooling, Fault };

struct Limits {
  float heat_below;
  float cool_at_or_above;
  float hysteresis;
};

class Thermostat {
 public:
  explicit Thermostat(const Limits& limits) : limits_(limits) {}

  // An invalid reading puts the thermostat in Fault, which switches everything off.
  State update(float temperature_c, bool valid) {
    if (!valid) {
      state_ = State::Fault;
      return state_;
    }

    const bool keep_heating =
        state_ == State::Heating && temperature_c < limits_.heat_below + limits_.hysteresis;
    const bool keep_cooling =
        state_ == State::Cooling && temperature_c > limits_.cool_at_or_above - limits_.hysteresis;

    if (!keep_heating && !keep_cooling) {
      if (temperature_c >= limits_.cool_at_or_above) {
        state_ = State::Cooling;
      } else if (temperature_c < limits_.heat_below) {
        state_ = State::Heating;
      } else {
        state_ = State::Idle;
      }
    }
    return state_;
  }

  State state() const { return state_; }

 private:
  Limits limits_;
  State state_ = State::Idle;
};

inline bool heater_on(State state) { return state == State::Heating; }
inline bool cooler_on(State state) { return state == State::Cooling; }

inline const char* name(State state) {
  switch (state) {
    case State::Idle:
      return "Idle";
    case State::Heating:
      return "Heating";
    case State::Cooling:
      return "Cooling";
    case State::Fault:
      return "Sensor error";
  }
  return "";
}

}  // namespace incubator
