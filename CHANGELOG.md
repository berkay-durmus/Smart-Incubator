# Changelog

Notable changes to this project. The format follows
[Keep a Changelog](https://keepachangelog.com/en/1.1.0/), and versions follow
[Semantic Versioning](https://semver.org/).

## [1.0.0]

The original single-file sketch (`main.ino`) is replaced by a structured firmware project.

### Added
- Control logic separated from the hardware (`thermostat.h`) with host-side unit tests.
- Hysteresis, so a relay no longer chatters at a threshold.
- Fail-safe: an invalid sensor reading switches both relays off and is shown on the display.
- CSV output on the serial port (`temperature_c,humidity_pct,state`).
- `config.h` for pins, limits and timing; `Makefile`, CI (unit tests and a compile check),
  MIT license, README with wiring and troubleshooting.
- Schematic and PCB images with English labels (`docs/images/`).

### Changed
- The DHT11 is read with the maintained Adafruit *DHT sensor library*; the `dht11.h` header the
  original used is not available from the Arduino Library Manager and the sketch did not compile
  with it.
- The loop no longer blocks in `delay()`.
- Relays are driven to their off level before the pins become outputs, so they do not pulse at
  power-up.

### Fixed
- The display showed garbled text: output was never cleared, `println` sent line breaks to the
  LCD, the humidity label had a stray `)` and the first line was wider than 16 columns.
- The result of the sensor read was ignored, so a failed read acted on stale or zero values.
- Redundant third condition and a duplicate `pinMode(2, INPUT)`.
