#pragma once
#include "I2C.hpp"

using namespace std;
using namespace Components;

namespace CO2Detection
{
    class SCD4xSensor
    {
    private:
        shared_ptr<I2CMaster> i2cMasterBus;
        uint16_t co2 = {0};
        int32_t temperature = {0};
        int32_t humidity = {0};
    public:
        SCD4xSensor(shared_ptr<I2CMaster> i2cMasterBus)
        {
            this->i2cMasterBus = i2cMasterBus;
        }
        shared_ptr<I2CMaster> getBus()
        {
            return this->i2cMasterBus;
        }
        esp_err_t init();
        esp_err_t readCO2();
        constexpr void setCO2(uint16_t co2, int32_t temperature, int32_t humidity)
        {
            this->co2 = co2;
            this->temperature = temperature;
            this->humidity = humidity;
        };
        constexpr uint16_t getCO2()
        {
            return co2;
        }
        constexpr int32_t getTemperature()
        {
            return temperature;
        };
        constexpr int32_t getHumidity()
        {
            return humidity;
        };
    };
}
