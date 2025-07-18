#if defined(ESP8266)
    #include <ESP8266WiFi.h>
#elif defined(ESP32)
    #include <WiFi.h>
#endif

#include "classes.hpp"
#include "helper.hpp"

namespace Sensors {
    TurbiditySensor::TurbiditySensor(uint8_t analogPin, uint16_t rawMin, uint16_t rawMax)
    :  _pin(analogPin), _rawMin(rawMin), _rawMax(rawMax), _rawADC(0) {}

    uint16_t TurbiditySensor::getRawADC() {
        return _rawADC;
    }

    float TurbiditySensor::getVoltage(uint16_t maxADC, float vRef) {
        return _rawADC / static_cast<float>(maxADC) * vRef;
    }

    uint8_t TurbiditySensor::getLevel() {
        return math::mapClampedInput<uint16_t, uint8_t>(_rawADC, _rawMin, _rawMax, 5, 1);
    }

    float TurbiditySensor::getPercentage() {
        return math::mapClampedInput<uint16_t, float>(_rawADC, _rawMin, _rawMax, 100, 1);
    }

    void TurbiditySensor::update() {
        _rawADC = analogRead(_pin);
    }

} // namespace Sensors

namespace Communication {
    WiFiManager::WiFiManager() = default;

    bool WiFiManager::begin(const char* ssid, const char* password) {
        
        WiFi.disconnect(true, true);
        WiFi.begin(ssid, password);

        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
        }

        return true;
    }

    BlynkManager::BlynkManager() = default;
    bool BlynkManager::begin() {
        return true;
    }

} // namespace Network