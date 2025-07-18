#include "classes.hpp"

Sensors::TurbiditySensor turbidity(A0, 405, 650);
Communication::WiFiManager wifi;

void setup() {
  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);

  if (wifi.begin("Infinix HOT 11 Play", "puyengey")) {
    digitalWrite(LED_BUILTIN, HIGH);
  }
}

void loop() {
  turbidity.update();

  Serial.print("Raw ADC   : "); Serial.println(turbidity.getRawADC());
  Serial.print("Voltage   : "); Serial.println(turbidity.getVoltage());
  Serial.print("Level     : "); Serial.println(turbidity.getLevel());
  Serial.print("Persentase: "); Serial.print(turbidity.getPercentage()); Serial.println("%\n");

  delay(1000);
}
