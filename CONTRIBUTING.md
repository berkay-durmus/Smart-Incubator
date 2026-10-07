# Contributing

```bash
git clone https://github.com/berkay-durmus/Smart-Incubator.git
cd Smart-Incubator
make libs      # needs arduino-cli
make test      # unit tests of the control logic
make compile   # build for an Arduino Uno
```

- Work on a branch and open a pull request; do not push to `main`.
- Control behaviour belongs in `thermostat.h` and needs a test in `test/test_thermostat.cpp`.
- Pins, limits and timing belong in `config.h`.
- Add a line to the `[Unreleased]` section of `CHANGELOG.md`.
