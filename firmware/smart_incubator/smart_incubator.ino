#include <DHT.h>
#include <LiquidCrystal_I2C.h>

#include "config.h"
#include "thermostat.h"

DHT dht(PIN_DHT, DHT11);
LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);
incubator::Thermostat thermostat({HEAT_BELOW_C, COOL_AT_OR_ABOVE_C, HYSTERESIS_C});

unsigned long last_sample_ms = 0;

void setRelay(uint8_t pin, bool on) {
  digitalWrite(pin, (on != RELAY_ACTIVE_LOW) ? HIGH : LOW);
}

void initRelay(uint8_t pin) {
  setRelay(pin, false);  // preset the level before enabling the output
  pinMode(pin, OUTPUT);
}

void printRow(uint8_t row, const char* text) {
  char line[LCD_COLUMNS + 1];
  snprintf(line, sizeof line, "%-*s", LCD_COLUMNS, text);
  lcd.setCursor(0, row);
  lcd.print(line);
}

void showReading(float temperature, float humidity, incubator::State state) {
  char top[LCD_COLUMNS + 1] = "";
  if (state != incubator::State::Fault) {
    char value[8];
    dtostrf(temperature, 5, 1, value);
    snprintf(top, sizeof top, "T:%sC H:%3d%%", value, static_cast<int>(humidity));
  }
  printRow(0, top);
  printRow(1, incubator::name(state));
}

void setup() {
  initRelay(PIN_HEATER_RELAY);
  initRelay(PIN_COOLER_RELAY);

  Serial.begin(SERIAL_BAUD);
  Serial.println(F("temperature_c,humidity_pct,state"));

  dht.begin();
  lcd.init();
  lcd.backlight();
  lcd.clear();
}

void loop() {
  if (millis() - last_sample_ms < SAMPLE_INTERVAL_MS) {
    return;
  }
  last_sample_ms = millis();

  const float temperature = dht.readTemperature();
  const float humidity = dht.readHumidity();
  const bool valid = !isnan(temperature) && !isnan(humidity);

  const incubator::State state = thermostat.update(temperature, valid);
  setRelay(PIN_HEATER_RELAY, incubator::heater_on(state));
  setRelay(PIN_COOLER_RELAY, incubator::cooler_on(state));

  showReading(temperature, humidity, state);

  Serial.print(valid ? temperature : 0.0f, 1);
  Serial.print(',');
  Serial.print(valid ? humidity : 0.0f, 0);
  Serial.print(',');
  Serial.println(incubator::name(state));
}
