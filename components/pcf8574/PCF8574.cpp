// PCF8574.cpp
#include "PCF8574.h"
#include "esp_err.h"

namespace kc868 {
    class I2CManager;
}

PCF8574::PCF8574(kc868::I2CManager &i2c_manager, const uint8_t device_addr)
    : i2c_manager_(i2c_manager), device_addr_(device_addr), state_(0xFF) {
} // Initialize with all HIGH (inputs)

esp_err_t PCF8574::write(const uint8_t data) {
    state_ = data;
    const esp_err_t ret = i2c_manager_.write(device_addr_, &data, 1);
    if (ret != ESP_OK) {
        ESP_LOGE("PCF8574", "Write failed to address 0x%02X with data 0x%02X", device_addr_, data);
    }
    return ret;
}

esp_err_t PCF8574::read(uint8_t &data) const {
    const esp_err_t ret = i2c_manager_.read(device_addr_, &data, 1);
    if (ret != ESP_OK) {
        ESP_LOGE("PCF8574", "Read failed from address 0x%02X", device_addr_);
    }
    return ret;
}

esp_err_t PCF8574::setPin(const uint8_t pin, const bool level) {
    if (level) {
        state_ |= (1 << pin);
    } else {
        state_ &= ~(1 << pin);
    }
    return write(state_);
}

esp_err_t PCF8574::getPin(const uint8_t pin, bool &level) const {
    uint8_t data;
    const esp_err_t ret = read(data);

    if (ret == ESP_OK) {
        level = data & (1 << pin);
    }
    return ret;
}
