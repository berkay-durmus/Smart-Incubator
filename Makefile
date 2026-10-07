FQBN ?= arduino:avr:uno
PORT ?= /dev/ttyACM0
SKETCH := firmware/smart_incubator

.PHONY: libs test compile upload monitor clean

libs:
	arduino-cli core install arduino:avr
	arduino-cli lib install "DHT sensor library" "Adafruit Unified Sensor" "LiquidCrystal I2C"

test:
	mkdir -p build
	g++ -std=c++17 -Wall -Wextra -Werror -I$(SKETCH) test/test_thermostat.cpp -o build/test_thermostat
	./build/test_thermostat

compile:
	arduino-cli compile --fqbn $(FQBN) $(SKETCH)

upload: compile
	arduino-cli upload --fqbn $(FQBN) --port $(PORT) $(SKETCH)

monitor:
	arduino-cli monitor --port $(PORT) --config baudrate=9600

clean:
	rm -rf build
