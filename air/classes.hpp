#pragma once

#include <ESP8266WiFi.h>
#include <functional>
#include "helper.hpp"

class TurbiditySensor {
    public:
        TurbiditySensor(uint8_t analogPin, uint16_t rawMin, uint16_t rawMax, uint8_t ADCRes, float vRef)
        :  _pin(analogPin), _rawMin(rawMin), _rawMax(rawMax), _rawADC(0), _ADCRes(ADCRes), _vRef(vRef), _rawVoltage(0) {
            _maxADC = pow(2, _ADCRes) - 1;
        }

        uint16_t getRawADC() {
            return _rawADC;
        }

        float getVoltage() {
            return _rawVoltage;
        }

        /**
        * Mengambil nilai turbiditas air dalam bentuk skala.
        * 
        * Nilai dihitung dari hasil pembacaan ADC (_rawADC) dengan 
        * memetakan nilai tersebut dari rentang [_rawMin, _rawMax] 
        * ke rentang 5-1, di mana 5 berarti keruh dan 1 sangat bersih.
        * 
        * @return Nilai turbiditas dalam skala 5-1.
        */
        uint8_t getLevel() {
            return mapClampedInput<uint16_t, uint8_t>(_rawADC, _rawMin, _rawMax, 5, 1);
        }

        /**
        * Mengambil nilai turbiditas air dalam bentuk persentase.
        * 
        * Nilai dihitung dari hasil pembacaan ADC (_rawADC) dengan 
        * memetakan nilai tersebut dari rentang [_rawMin, _rawMax] 
        * ke rentang 100–0%, di mana 100% berarti keruh dan 0% sangat bersih.
        * 
        * @return Nilai turbiditas dalam persentase (%).
        */
        float getPercentage() {
            return mapClampedInput<uint16_t, float>(_rawADC, _rawMin, _rawMax, 100, 0);
        }

        /**
        * Mengambil nilai turbiditas air dalam satuan NTU (mg/L).
        * 
        * Nilai dihitung berdasarkan tegangan analog (_rawVoltage) menggunakan
        * persamaan regresi kuadratik dari sensor DFRobot Turbidity (SKU: SEN0189).
        * Persamaan ini diperoleh dari dokumentasi resmi DFRobot:
        * https://wiki.dfrobot.com/Turbidity_sensor_SKU__SEN0189
        * 
        * @return Nilai turbiditas dalam satuan NTU (Nephelometric Turbidity Unit).
        */
        float getNTU() {
            return -1120.42 * (_rawVoltage * _rawVoltage) 
                    + 5742.3*(_rawVoltage) 
                    - 4352.9;
        }

        void update() {
            _rawADC = analogRead(_pin);
            _rawVoltage = _rawADC / _maxADC * _vRef;
        }

    private:
        uint8_t _pin;
        uint16_t _rawADC;
        uint16_t _rawMin;
        uint16_t _rawMax;
        float _rawVoltage;

        uint8_t _ADCRes;
        double _maxADC;
        float _vRef;
};

class WiFiManager {
    public:
        WiFiManager() = default;

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

class BlynkManager {
    public:
        BlynkManager() = default;

        bool begin() {
            return true;
        }
};

class Timer {

    public:
        Timer(unsigned long interval, std::function<void()> callback)
        : _interval(interval), _callback(callback), _prevMillis(0) {}

        void update() {
            unsigned long currentMillis = millis();
            if (currentMillis - _prevMillis >= _interval) {
                _prevMillis = currentMillis;
                _callback();
            }
        }

        void setInterval(unsigned long newInterval) {
            _interval = newInterval;
        }

        void reset() {
            _prevMillis = millis();
        }

    private:
        unsigned long _interval;
        unsigned long _prevMillis;
        std::function<void()> _callback;
};

