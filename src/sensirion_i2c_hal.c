#include "sensirion_i2c_hal.h"
#include "../driver/sensirion_common.h"
#include "../driver/sensirion_config.h"
#include "I2C.hpp"
#include "freertos/Freertos.h"
#include "freertos/task.h"
#include "esp_timer.h"
#include <memory>
#include <cstring>


CO2Detection::SCD4xSensor *_scd4xSensor;
/**
 * Select the current i2c bus by index.
 * All following i2c operations will be directed at that bus.
 *
 * THE IMPLEMENTATION IS OPTIONAL ON SINGLE-BUS SETUPS (all sensors on the same
 * bus)
 *
 * @param bus_idx   Bus index to select
 * @returns         0 on success, an error code otherwise
 */
int16_t sensirion_i2c_hal_select_bus(uint8_t bus_idx)
{
    /* TODO:IMPLEMENT or leave empty if all sensors are located on one single
     * bus
     */
    return NO_ERROR;
}

/**
 * Initialize all hard- and software components that are needed for the I2C
 * communication.
 */
void sensirion_i2c_hal_init(CO2Detection::SCD4xSensor *co2Sensor)
{
    _scd4xSensor = co2Sensor;
}

/**
 * Release all resources initialized by sensirion_i2c_hal_init().
 */
void sensirion_i2c_hal_free(void)
{
    /* TODO:IMPLEMENT or leave empty if no resources need to be freed */
}

/**
 * Execute one read transaction on the I2C bus, reading a given number of bytes.
 * If the device does not acknowledge the read command, an error shall be
 * returned.
 *
 * @param address 7-bit I2C address to read from
 * @param data    pointer to the buffer where the data is to be stored
 * @param count   number of bytes to read from I2C and store in the buffer
 * @returns 0 on success, error code otherwise
 */
int8_t sensirion_i2c_hal_read(uint8_t address, uint8_t *data, uint16_t count)
{
    try
    {
        _scd4xSensor->getBus()->syncWrite(I2CAddress(address), {address});
        vector<uint8_t> dataVector = _scd4xSensor->getBus()->syncRead(I2CAddress(address), count);
        memcpy(data, dataVector.data(), count);
        return ESP_OK;
    }
    catch (const I2CException &e)
    {
        ESP_LOGI("TAG", "I2C Exception with error: %s (0x%X)", e.what(), e.error);
        ESP_LOGI("TAG", "Couldn't read sensor!");
        return ESP_FAIL;
    }
}

/**
 * Execute one write transaction on the I2C bus, sending a given number of
 * bytes. The bytes in the supplied buffer must be sent to the given address. If
 * the slave device does not acknowledge any of the bytes, an error shall be
 * returned.
 *
 * @param address 7-bit I2C address to write to
 * @param data    pointer to the buffer containing the data to write
 * @param count   number of bytes to read from the buffer and send over I2C
 * @returns 0 on success, error code otherwise
 */
int8_t sensirion_i2c_hal_write(uint8_t address, const uint8_t *data,
                               uint16_t count)
{
    try
    {
        vector<uint8_t> dataVector;
        dataVector.push_back(address);
        for (int i = 0; i < len; i++)
        {
            dataVector.push_back(data[i]);
        }
        _scd4xSensor->getBus()->syncWrite(I2CAddress(address), dataVector);
        return ESP_OK;
    }
    catch (const I2CException &e)
    {
        ESP_LOGI("TAG", "I2C Exception with error: %s (0x%X)", e.what(), e.error);
        ESP_LOGI("TAG", "Couldn't write sensor!");
        return ESP_FAIL;
    }
}

/**
 * Sleep for a given number of microseconds. The function should delay the
 * execution for at least the given time, but may also sleep longer.
 *
 * Despite the unit, a <10 millisecond precision is sufficient.
 *
 * @param useconds the sleep time in microseconds
 */
void sensirion_i2c_hal_sleep_usec(uint32_t useconds)
{
    vTaskDelay(pdMS_TO_TICKS(useconds / 1000));
}
