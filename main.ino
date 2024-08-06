#include <LiquidCrystal_I2C.h>
#include <dht11.h>
#define DHT11PIN 2
#define DELAY 2000


dht11 DHT11;
LiquidCrystal_I2C lcdScreen(0x27, 16, 2);

void setup() {

  lcdScreen.init();
  lcdScreen.backlight();
  lcdScreen.clear();
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(2, INPUT);
}

void loop()
{
  int chk = DHT11.read(DHT11PIN);

  lcdScreen.print("Temperature: ");
  lcdScreen.println((float)DHT11.temperature, 2);
  lcdScreen.setCursor(0, 0);
  lcdScreen.print("Humidity (%): )");
  lcdScreen.println((float)DHT11.humidity, 2);
  lcdScreen.setCursor(0, 1);
 
  if (DHT11.temperature>=39)
  {
    digitalWrite(11, LOW);
    digitalWrite(10, HIGH);
    delay(DELAY);
  }
  else if(DHT11.temperature<36)
  {
    digitalWrite(10, LOW);
    digitalWrite(11, HIGH);
    delay(DELAY);
  }
  else if(DHT11.temperature<=39 and DHT11.temperature>=36)
  {
    digitalWrite(10, HIGH);
    digitalWrite(11, HIGH);
    delay(DELAY);
  }
}
