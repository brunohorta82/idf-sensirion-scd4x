
#include "SCD4xSensor.hpp"
#include "SensirionSCD4xInterface.hpp"
#include "freertos/Freertos.h"
#include "freertos/task.h"
#include <esp_log.h>
#include "I2C.hpp"
#include "sensirion_i2c_hal.hpp"
#include "../driver/scd4x_i2c.hpp"
#include "../driver/sensirion_common.hpp"

#define NUM_USED_OUTPUTS 9

using namespace std;

CO2Detection::SCD4xSensor *_co2Sensor;

esp_err_t initSCD4x(CO2Detection::SCD4xSensor *co2Sensor)
{
    int16_t error = 0;
    _co2Sensor = co2Sensor;
    sensirion_i2c_hal_sleep_usec(1000000);
    
    sensirion_i2c_hal_init(co2Sensor);

    // Clean up potential SCD40 states
    scd4x_wake_up();
    scd4x_stop_periodic_measurement();
    scd4x_reinit();

    uint16_t serial_0;
    uint16_t serial_1;
    uint16_t serial_2;
    error = scd4x_get_serial_number(&serial_0, &serial_1, &serial_2);
    if (error)
    {
        printf("Error executing scd4x_get_serial_number(): %i\n", error);
    }
    else
    {
        printf("serial: 0x%04x%04x%04x\n", serial_0, serial_1, serial_2);
    }

    // Start Measurement

    error = scd4x_start_periodic_measurement();
    if (error)
    {
        printf("Error executing scd4x_start_periodic_measurement(): %i\n", error);
    }

    printf("Waiting for first measurement... (5 sec)\n");

    return ESP_OK;
}

esp_err_t readSCD4x()
{
    int16_t error = 0;
    bool data_ready_flag = false;
    error = scd4x_get_data_ready_flag(&data_ready_flag);
    if (error)
    {
        printf("Error executing scd4x_get_data_ready_flag(): %i\n", error);
        return ESP_FAIL;
    }
    if (!data_ready_flag)
    {
        return ESP_FAIL;
    }

    uint16_t co2;
    int32_t temperature;
    int32_t humidity;
    error = scd4x_read_measurement(&co2, &temperature, &humidity);
    if (error)
    {
        printf("Error executing scd4x_read_measurement(): %i\n", error);
        return ESP_FAIL;
    }
    if (co2 == 0)
    {
        printf("Invalid sample detected, skipping.\n");
        return ESP_FAIL;
    }

    printf("CO2: %u\n", co2);
    printf("Temperature: %ld m°C\n", temperature);
    printf("Humidity: %ld mRH\n", humidity);
    _co2Sensor->setCO2(co2, temperature, humidity);
    return ESP_OK;
}