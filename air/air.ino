#include "classes.hpp"

TurbiditySensor turbidity(A0, 405, 650, 10, 5.0);
void printData() {
  turbidity.update();

  Serial.print("Raw ADC   : "); Serial.println(turbidity.getRawADC());
  Serial.print("Voltage   : "); Serial.print(turbidity.getVoltage(), 3);    Serial.println(" V");
  Serial.print("Level     : "); Serial.println(turbidity.getLevel());
  Serial.print("Turbiditas: "); Serial.print(turbidity.getNTU(), 3);        Serial.println(" NTU");
  Serial.print("Persentase: "); Serial.print(turbidity.getPercentage(), 3); Serial.println("%\n");
}

Timer timer(1000, printData);
WiFiManager wifi;

void setup() {
  Serial.begin(9600);

  wifi.begin("Infinix HOT 11 Play", "puyengey");
}

void loop() {
  timer.update();
}
