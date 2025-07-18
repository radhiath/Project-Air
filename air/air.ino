#include "classes.hpp"

Sensors::TurbiditySensor turbidity(A0, 405, 650);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

}

void loop() {
  turbidity.update();

  Serial.print("Raw ADC   : "); Serial.println(turbidity.getRawADC());
  Serial.print("Voltage   : "); Serial.println(turbidity.getVoltage());
  Serial.print("Level     : "); Serial.println(turbidity.getLevel());
  Serial.print("Persentase: "); Serial.print(turbidity.getPercentage()); Serial.println("%\n");

  delay(1000);
}
