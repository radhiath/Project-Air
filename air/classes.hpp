#pragma once

#include <Arduino.h>

namespace Sensors {
    class TurbiditySensor {
        public:
            TurbiditySensor(uint8_t analogPin, uint16_t rawMin, uint16_t rawMax);
            uint16_t getRawADC();
            float getVoltage(uint16_t maxADC = 1023, float vRef = 5.0);
            uint8_t getLevel();
            float getPercentage();
            void update();

        private:
            uint8_t _pin;
            uint16_t _rawADC;
            uint16_t _rawMin;
            uint16_t _rawMax;
            
    };

} // namespace Sensors

namespace Communication {
    class WiFiManager {
        public:
            WiFiManager();
            bool begin(const char* ssid, const char* password);
    };

    class BlynkManager {
        public:
            BlynkManager();
            bool begin();
    };
} // namespace Network