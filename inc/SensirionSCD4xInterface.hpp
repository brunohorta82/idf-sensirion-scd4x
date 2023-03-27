#pragma once
#include "SCD4xSensor.hpp"
using namespace std;

esp_err_t initSCD4x(CO2Detection::SCD4xSensor *co2Sensor);
void startSCD4xLoop();
