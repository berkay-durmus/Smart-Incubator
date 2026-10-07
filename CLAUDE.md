# CLAUDE.md

Arduino Uno firmware for a temperature-controlled incubator: a DHT11 sensor, a 16x2 I2C LCD and
two relays (heater, cooler). The README is the source of truth for wiring and configuration.

## Commands

```bash
make libs      # arduino-cli core + libraries
make test      # g++ unit tests of thermostat.h (no Arduino needed)
make compile   # arduino-cli compile for arduino:avr:uno
make upload PORT=/dev/ttyACM0
```

## Architecture

`firmware/smart_incubator/thermostat.h` holds the control logic and has no Arduino dependency,
so `test/test_thermostat.cpp` can run on a PC. `smart_incubator.ino` is only glue: sample every
`SAMPLE_INTERVAL_MS`, ask the thermostat, drive the relays, update the LCD and serial port.
All pins and limits live in `config.h`.

## Conventions

- Relays are active-low (`RELAY_ACTIVE_LOW`). An invalid sensor reading must switch both off.
- Keep comments short; everything user-facing is in English.
- Never push to `main`; use a branch.
