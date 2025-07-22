// Include custom classes
#include "classes.hpp"
#include "helper.hpp"
#include "secrets.hpp"

#include <BlynkSimpleEsp8266.h>

// Instatiate objek TurbiditySensor dengan nama turbidity
TurbiditySensor turbidity(A0, 405, 650, 10, 5.0);

// Define fungsi printData sebagai callback
void printData() {
  turbidity.update();

  // Print ke Serial Monitor
  DEBUG_PRINT("Raw ADC   : "); DEBUG_PRINTLN(turbidity.getRawADC());
  DEBUG_PRINT("Voltage   : "); DEBUG_PRINT(turbidity.getVoltage(), 3);    DEBUG_PRINTLN(" V");
  DEBUG_PRINT("Level     : "); DEBUG_PRINTLN(turbidity.getLevel());
  DEBUG_PRINT("Turbiditas: "); DEBUG_PRINT(turbidity.getNTU(), 3);        DEBUG_PRINTLN(" NTU");
  DEBUG_PRINT("Persentase: "); DEBUG_PRINT(turbidity.getPercentage(), 3); DEBUG_PRINTLN("%\n");

  // Kirim ke Blynk (misalnya ke Virtual Pin V0)
  Blynk.virtualWrite(V0, turbidity.getRawADC());
  Blynk.virtualWrite(V1, turbidity.getVoltage());
  Blynk.virtualWrite(V2, turbidity.getLevel());
  Blynk.virtualWrite(V3, turbidity.getPercentage());
}

// Instatiate objek Timer dengan nama timer
Timer timer(2000, printData); // Timer 2 detik
// Instatiate objek WiFiManager dengan nama wifi
WiFiManager wifi;

// =====================
// MAIN PROGRAM
// =====================
void setup() {
  // Setup Serial Monitor dengan baud rate 9600
  DEBUG_BEGIN(9600);

  // Memulai koneksi WiFi
  wifi.begin(ssid, pass);

  // Koneksi ke Blynk cloud
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop() {
  // Jalankan Blynk
  Blynk.run();

  // Update timer
  timer.update();
}
