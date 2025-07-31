// Header guard
#pragma once

// Include library yang digunakan
#include <ESP8266WiFi.h> // Library WiFi
#include <functional>    // Library C++ (digunakan pada class Timer)
#include "helper.hpp"    // Library buatan sendiri

/**
 * Kelas TurbiditySensor digunakan untuk membaca dan mengonversi data dari Turbidity Sensor.
 */
class TurbiditySensor {
    public:
        /**
         * Konstruktor untuk inisialisasi sensor turbiditas.
         * 
         * @param analogPin Pin analog yang terhubung ke sensor.
         * @param rawMin Nilai ADC minimum dari sensor (air keruh).
         * @param rawMax Nilai ADC maksimum dari sensor (air bersih).
         * @param ADCRes Resolusi ADC, default = 10 bit.
         * @param vRef Tegangan referensi untuk konversi tegangan, default = 5.0V.
         */
        TurbiditySensor(uint8_t analogPin, uint16_t rawMin, uint16_t rawMax, uint8_t ADCRes = 10, float vRef = 5.0)
        :  _pin(analogPin), _rawMin(rawMin), _rawMax(rawMax), _rawADC(0), _ADCRes(ADCRes), _vRef(vRef), _rawVoltage(0) {
            _maxADC = pow(2, _ADCRes) - 1;
        }

        /**
         * Mengambil nilai mentah dari ADC.
         * 
         * @return Nilai ADC mentah (0 - nilai maksimum ADC).
         */
        uint16_t getRawADC() {
            return _rawADC;
        }

        /**
         * Mengambil nilai tegangan hasil konversi dari ADC.
         * 
         * Tegangan dihitung berdasarkan rasio nilai ADC terhadap maksimum ADC,
         * dikalikan dengan tegangan referensi (vRef).
         * 
         * @return Nilai tegangan dalam volt (V).
         */
        float getVoltage() {
            return _rawVoltage;
        }

        /**
         * Mengambil nilai turbiditas air dalam bentuk skala.
         * 
         * Nilai dihitung dari hasil pembacaan ADC (_rawADC) dengan 
         * memetakan nilai tersebut dari rentang [_rawMin, _rawMax] 
         * ke rentang 1-5, di mana 1 berarti keruh dan 5 sangat bersih.
         * 
         * @return Nilai turbiditas dalam skala 1-5.
         */
        uint8_t getLevel() {
            if (_rawADC >= 641 && _rawADC <= 650) {
                return 1; // Sangat bersih
            } else if (_rawADC >= 621 && _rawADC <= 640) {
                return 2;
            } else if (_rawADC >= 601 && _rawADC <= 620) {
                return 3;
            } else if (_rawADC >= 461 && _rawADC <= 600) {
                return 4;
            } else if (_rawADC >= 447 && _rawADC <= 460) {
                return 5; // Sangat keruh
            }
            return 0; // Di luar range
        }

        // DEPRECATED
        /**
         * Mengambil nilai turbiditas air dalam bentuk persentase.
         * 
         * Nilai dihitung dari hasil pembacaan ADC (_rawADC) dengan 
         * memetakan nilai tersebut dari rentang [_rawMin, _rawMax] 
         * ke rentang 100–0%, di mana 100% berarti keruh dan 0% sangat bersih.
         * 
         * @return Nilai turbiditas dalam persentase (%).
         */
        // float getPercentage() {
        //     return mapClampedInput<uint16_t, float>(_rawADC, _rawMin, _rawMax, 0, 100);
        // }

        // DEPRECATED
        /**
         * Mengambil nilai turbiditas air dalam satuan NTU (mg/L) (EKSPERIMENTAL).
         * 
         * Nilai dihitung berdasarkan tegangan analog (_rawVoltage) menggunakan
         * persamaan regresi kuadratik dari sensor DFRobot Turbidity (SKU: SEN0189).
         * Persamaan ini diperoleh dari dokumentasi resmi DFRobot:
         * https://wiki.dfrobot.com/Turbidity_sensor_SKU__SEN0189
         * 
         * @return Nilai turbiditas dalam satuan NTU (Nephelometric Turbidity Unit).
         */
        // float getNTU() {
        //     return -1120.42 * (_rawVoltage * _rawVoltage) 
        //             + 5742.3*(_rawVoltage) 
        //             - 4352.9;
        // }

        /**
        * Mengambil deskripsi kondisi air dalam bentuk string.
        * 
        * Deskripsi ini ditentukan berdasarkan level turbiditas air (1–5)
        * yang diperoleh dari hasil pemetaan nilai ADC. Nilai level 1 menunjukkan
        * air sangat bersih, sedangkan level 5 menunjukkan air sangat keruh.
        * 
        * @return Pointer ke string literal yang merepresentasikan kondisi air.
        */
        const char* getCondition() {
            uint8_t level = getLevel();

            switch (level) {
                case 1: return "Sangat Keruh";
                case 2: return "Keruh";
                case 3: return "Agak Keruh";
                case 4: return "Bersih";
                case 5: return "Sangat Bersih";
                default: return "Tidak Diketahui"; // In case muncul unwanted behavior
            }
        }

        /**
         * Memperbarui nilai ADC dan tegangan dari sensor.
         * 
         * Fungsi ini perlu dipanggil secara berkala untuk memastikan nilai diupdate.
         */
        void update() {
            _rawADC = analogRead(_pin);
            _rawVoltage = _rawADC / _maxADC * _vRef;
        }

    private:
        uint8_t _pin;      // Pin analog yang digunakan.
        uint16_t _rawADC;  // Nilai mentah ADC.
        uint16_t _rawMin;  // Nilai ADC minimum dari sensor.
        uint16_t _rawMax;  // Nilai ADC maksimum dari sensor.
        float _rawVoltage; // Nilai tegangan mentah.

        uint8_t _ADCRes;   // Resolusi ADC.
        double _maxADC;    // Nilai ADC maksimum.
        float _vRef;       // Tegangan referensi
};

/**
 * Kelas WiFiManager digunakan untuk mengelola koneksi WiFi.
 */
class WiFiManager {
    public:
        /**
         * Konstruktor untuk inisialisasi WiFi (default constructor).
         */
        WiFiManager() = default;

        /**
         * Memulai koneksi WiFi dengan SSID dan password yang diberikan.
         * 
         * Fungsi akan mencoba menyambungkan perangkat ke jaringan WiFi hingga
         * maksimal 5 kali percobaan, masing-masing dengan jeda 500 ms.
         * 
         * @param ssid Nama SSID WiFi.
         * @param password Password WiFi.
         * @return True jika berhasil terhubung, false jika gagal.
         */
        bool begin(const char* ssid, const char* password) {
            const uint8_t maxRetries = 5;
            const uint16_t retryDelayMs = 500;

            WiFi.disconnect(true, true);
            WiFi.mode(WIFI_STA);
            WiFi.begin(ssid, password);

            uint8_t counter = 0;
            while (WiFi.status() != WL_CONNECTED && counter < maxRetries) {
                delay(retryDelayMs);
                counter++;
            }

            return WiFi.status() == WL_CONNECTED;
        }
};

/**
 * Kelas Timer digunakan untuk menjalankan fungsi callback secara periodik
 * tanpa menggunakan delay blocking.
 */
class Timer {

    public:
        /**
         * Konstruktor Timer.
         * 
         * @param interval Interval waktu antar callback dalam milidetik.
         * @param callback Fungsi yang akan dipanggil setiap interval.
         */
        Timer(unsigned long interval, std::function<void()> callback)
        : _interval(interval), _callback(callback), _prevMillis(0) {}

        /**
         * Memeriksa apakah waktu interval telah tercapai, dan jika ya,
         * akan memanggil fungsi callback yang telah ditentukan.
         * 
         * Fungsi ini harus dipanggil secara berkala.
         */
        void update() {
            unsigned long currentMillis = millis();
            if (currentMillis - _prevMillis >= _interval) {
                _prevMillis = currentMillis;
                _callback();
            }
        }

        /**
         * Mengatur ulang interval waktu baru untuk timer.
         * 
         * @param newInterval Nilai interval baru dalam milidetik.
         */
        void setInterval(unsigned long newInterval) {
            _interval = newInterval;
        }

        /**
         * Mereset waktu acuan timer ke waktu saat ini.
         * 
         * Dapat digunakan untuk memulai ulang perhitungan interval.
         */
        void reset() {
            _prevMillis = millis();
        }

    private:
        unsigned long _interval;         // Interval pemanggilan callback.
        unsigned long _prevMillis;       // Waktu pemanggilan callback sebelumnya.
        std::function<void()> _callback; // Callback void function/method/lambda.
};