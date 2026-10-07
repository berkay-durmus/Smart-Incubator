#pragma once

#include <stdint.h>

// Pins
constexpr uint8_t PIN_DHT = 2;
constexpr uint8_t PIN_HEATER_RELAY = 10;
constexpr uint8_t PIN_COOLER_RELAY = 11;

// A relay board that switches on when its input is pulled LOW
constexpr bool RELAY_ACTIVE_LOW = true;

// 16x2 LCD behind an I2C backpack
constexpr uint8_t LCD_ADDRESS = 0x27;
constexpr uint8_t LCD_COLUMNS = 16;
constexpr uint8_t LCD_ROWS = 2;

// Control band in degrees Celsius
constexpr float HEAT_BELOW_C = 36.0f;
constexpr float COOL_AT_OR_ABOVE_C = 39.0f;
constexpr float HYSTERESIS_C = 0.5f;

// The DHT11 cannot be read faster than once per second
constexpr unsigned long SAMPLE_INTERVAL_MS = 2000;
constexpr unsigned long SERIAL_BAUD = 9600;
