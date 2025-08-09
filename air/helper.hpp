// Header guard
#pragma once

#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>

#include "secrets.hpp"

// Ubah nilai MODE ke 1 untuk mengaktifkan mode debug (Serial.print aktif)
// MODE = 0 → mode final (tanpa output Serial)
// MODE = 1 → mode debug (output Serial aktif)
#define MODE 1

#if !MODE
#define FINAL_MODE   // Mode final aktif
#else
#define DEBUG_MODE   // Mode debug aktif
#endif

// ====================
// Macro Debug
// ====================
// Jika DEBUG_MODE aktif:
//   - Macro akan aktif untuk debugging lewat Serial
// Jika FINAL_MODE aktif:
//   - Macro tidak menghasilkan apa pun (tidak menambah ukuran program)

#ifdef DEBUG_MODE
#define DEBUG_BEGIN(baud)       Serial.begin(baud)
#define DEBUG_PRINT(...)        Serial.print(__VA_ARGS__)
#define DEBUG_PRINTLN(...)      Serial.println(__VA_ARGS__)
#else
#define DEBUG_BEGIN(baud)
#define DEBUG_PRINT(...)
#define DEBUG_PRINTLN(...)
#endif

// DEPRECATED
/**
 * Mengembalikan nilai yang telah dibatasi dalam rentang tertentu.
 * 
 * Fungsi ini membatasi nilai `x` agar tidak lebih kecil dari `lowerBound`
 * dan tidak lebih besar dari `upperBound`.
 * 
 * @tparam T Tipe data numerik (int, float, dll.).
 * @param x Nilai yang akan diperiksa.
 * @param lowerBound Batas bawah yang diperbolehkan.
 * @param upperBound Batas atas yang diperbolehkan.
 * @return Nilai x yang telah dikunci dalam rentang [lowerBound, upperBound].
 */
// template <typename T>
// T constrainValue(T x, T lowerBound, T upperBound) {
//     return (x < lowerBound) ? lowerBound : (x > upperBound) ? upperBound : x;
// }

// DEPRECATED
/**
 * Memetakan nilai dari satu rentang ke rentang lain.
 * 
 * Fungsi ini mengubah nilai `x` dari rentang asal [fromLow, fromHigh]
 * ke rentang tujuan [toLow, toHigh] secara proporsional.
 * 
 * Jika fromHigh == fromLow, maka nilai toLow akan dikembalikan untuk mencegah pembagian dengan nol.
 * 
 * @tparam InputType Tipe data input (biasanya int atau float).
 * @tparam OutputType Tipe data output yang diinginkan.
 * @param x Nilai yang akan dipetakan.
 * @param fromLow Batas bawah rentang asal.
 * @param fromHigh Batas atas rentang asal.
 * @param toLow Batas bawah rentang tujuan.
 * @param toHigh Batas atas rentang tujuan.
 * @return Nilai hasil pemetaan dalam rentang tujuan.
 */
// template <typename InputType, typename OutputType>
// OutputType mapValue(InputType x, InputType fromLow, InputType fromHigh, InputType toLow, InputType toHigh) {
//     if (fromHigh == fromLow) return static_cast<OutputType>(toLow);
//     return static_cast<OutputType>((x - fromLow) * ((toHigh - toLow) / static_cast<double>(fromHigh - fromLow)) + toLow);
// }

// DEPRECATED
/**
 * Memetakan nilai dari satu rentang ke rentang lain dengan input terkunci (clamped).
 * 
 * Nilai `x` terlebih dahulu dibatasi (clamp) ke dalam rentang asal [fromLow, fromHigh],
 * kemudian dipetakan ke rentang tujuan [toLow, toHigh] menggunakan pemetaan linear.
 * 
 * @tparam InputType Tipe data input (biasanya int atau float).
 * @tparam OutputType Tipe data output yang diinginkan.
 * @param x Nilai yang akan dipetakan.
 * @param fromLow Batas bawah rentang asal.
 * @param fromHigh Batas atas rentang asal.
 * @param toLow Batas bawah rentang tujuan.
 * @param toHigh Batas atas rentang tujuan.
 * @return Nilai hasil pemetaan dalam rentang tujuan, setelah dikunci di rentang asal.
 */
// template <typename InputType, typename OutputType>
// OutputType mapClampedInput(InputType x, InputType fromLow, InputType fromHigh, InputType toLow, InputType toHigh) {
//     auto clampedVal = constrainValue(x, fromLow, fromHigh);
//     return mapValue<InputType, OutputType>(clampedVal, fromLow, fromHigh, toLow, toHigh);
// }

String createJsonPayload(String time, uint16_t score, uint8_t level, float ntu, String condition) {
    StaticJsonDocument<300> doc;
    doc["time"] = time;
    doc["score"] = score;
    doc["level"] = level;
    doc["condition"] = condition;
    doc["ntu"] = ntu;

    String jsonStr;
    serializeJson(doc, jsonStr);

    DEBUG_PRINTLN("[DEBUG] JSON Payload dibuat");
    return jsonStr;
}

int sendDataToServer(const String &jsonPayload) {
    if (WiFi.status() == WL_CONNECTED) {
        WiFiClientSecure client;
        client.setInsecure();  // Bypass sertifikat SSL

        HTTPClient http;
        http.begin(client, serverName);
        http.addHeader("Content-Type", "application/json");

        int httpResponseCode = http.POST(jsonPayload);
        http.end();
        return httpResponseCode;
    }
    return -1; // Kalau koneksi Wi-Fi gagal
}