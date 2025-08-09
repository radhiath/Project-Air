#include "secrets.hpp"

#include <BlynkSimpleEsp8266.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

#include "classes.hpp"
#include "helper.hpp"

const uint8_t CLEANEST = 649;
const uint8_t DIRTIEST = 447;

TurbiditySensor turbidity(A0, DIRTIEST, CLEANEST, 10, 5.0);

uint16_t score = 0;
uint8_t level = 0;
String currentTime = "";
String condition = "";
float ntu = 0.0;

void printData(); // Declare prototype

Timer timer(10000, printData); // 2 detik interval
WiFiManager wifi;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 7 * 3600, 60000);

void setup() {
  DEBUG_BEGIN(9600);

  wifi.begin(ssid, pass);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  timeClient.begin();
}

void loop() {
  Blynk.run();
  timer.update();
}

void printData() {
  turbidity.update();
  timeClient.update();

  currentTime = timeClient.getFormattedTime();
  score = turbidity.getRawADC();
  level = turbidity.getLevel();
  condition = turbidity.getCondition();
  ntu = turbidity.getNTU();

  DEBUG_PRINTLN("=======================================================");
  DEBUG_PRINT("[DEBUG] Waktu       : "); DEBUG_PRINTLN(currentTime);
  DEBUG_PRINT("[DEBUG] Raw ADC     : "); DEBUG_PRINTLN(score);
  DEBUG_PRINT("[DEBUG] Level       : "); DEBUG_PRINTLN(level);
  DEBUG_PRINT("[DEBUG] Kondisi air : "); DEBUG_PRINTLN(condition);
  DEBUG_PRINT("[DEBUG] Kekeruhan   : "); DEBUG_PRINT(ntu); DEBUG_PRINTLN(" NTU");
 
  Blynk.virtualWrite(V0, score);
  Blynk.virtualWrite(V1, level);
  Blynk.virtualWrite(V2, condition);
  Blynk.virtualWrite(V3, ntu);

  String payload = createJsonPayload(currentTime, score, level, ntu, condition);
  if (WiFi.status() == WL_CONNECTED) {
    int response = sendDataToServer(payload);
    DEBUG_PRINT("[DEBUG] HTTP Response Code: ");
    DEBUG_PRINTLN(response);
    DEBUG_PRINTLN();
  } else {
    DEBUG_PRINTLN("[DEBUG] WiFi tidak terhubung!");
    DEBUG_PRINTLN();
  }
}