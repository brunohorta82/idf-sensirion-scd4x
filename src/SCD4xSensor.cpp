#include "SCD4xSensor.hpp"
#include "SensirionSCD4xInterface.hpp"

namespace CO2Detection
{
    esp_err_t SCD4xSensor::init()
    {
        
        return initSCD4x(this);
    }

    esp_err_t SCD4xSensor::readCO2()
    {
        return readSCD4x();
    }
}
