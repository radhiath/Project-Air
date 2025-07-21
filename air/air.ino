// Include library
#include "classes.hpp"

// Instatiate objek TurbiditySensor dengan nama turbidity
TurbiditySensor turbidity(A0, 405, 650, 10, 5.0);

// Define fungsi `printData` sebagai callback
void printData() {
  turbidity.update(); // Update data turbidity

  // Print data dari sensor
  Serial.print("Raw ADC   : "); Serial.println(turbidity.getRawADC());
  Serial.print("Voltage   : "); Serial.print(turbidity.getVoltage(), 3);    Serial.println(" V");
  Serial.print("Level     : "); Serial.println(turbidity.getLevel());
  Serial.print("Turbiditas: "); Serial.print(turbidity.getNTU(), 3);        Serial.println(" NTU");
  Serial.print("Persentase: "); Serial.print(turbidity.getPercentage(), 3); Serial.println("%\n");
}

// Instatiate objek Timer dengan nama timer
Timer timer(2000, printData);

// Instatiate objek WiFiManager dengan nama wifi
WiFiManager wifi;

// =====================
// MAIN PROGRAM
// =====================
void setup() {
  // Setup Serial Monitor dengan baud rate 9600
  Serial.begin(9600);

  // Memulia koneksi WiFi
  wifi.begin("Infinix HOT 11 Play", "puyengey");
}

void loop() {
  // Update timer
  timer.update();
}
