#include "SCD4xSensor.hpp"
#include "SensirionSCD4xInterface.hpp"

namespace CO2Detection
{
    esp_err_t SCD4xSensor::init()
    {
        
        return initSCD4x(this);
    }

    void SCD4xSensor::startSensorLoop()
    {
        startSCD4xLoop();
    }
}
