#include "driver/gpio.h"
#include "driver/ledc.h"
#include "esp_check.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

namespace {
constexpr gpio_num_t kMotorIn1 = GPIO_NUM_1;
constexpr gpio_num_t kMotorIn2 = GPIO_NUM_2;
constexpr gpio_num_t kLed = GPIO_NUM_20;
constexpr gpio_num_t kBuzzer = GPIO_NUM_21;
constexpr ledc_mode_t kBuzzerSpeedMode = LEDC_LOW_SPEED_MODE;
constexpr ledc_timer_t kBuzzerTimer = LEDC_TIMER_0;
constexpr ledc_channel_t kBuzzerChannel = LEDC_CHANNEL_0;
constexpr char kTag[] = "boot";
}  // namespace

extern "C" void app_main() {
    // Set both output latches low before enabling the pins as outputs.
    ESP_ERROR_CHECK(gpio_set_level(kMotorIn1, 0));
    ESP_ERROR_CHECK(gpio_set_level(kMotorIn2, 0));

    gpio_config_t motor_pins = {};
    motor_pins.pin_bit_mask = (1ULL << kMotorIn1) | (1ULL << kMotorIn2);
    motor_pins.mode = GPIO_MODE_OUTPUT;
    motor_pins.pull_up_en = GPIO_PULLUP_DISABLE;
    motor_pins.pull_down_en = GPIO_PULLDOWN_DISABLE;
    motor_pins.intr_type = GPIO_INTR_DISABLE;
    ESP_ERROR_CHECK(gpio_config(&motor_pins));

    ESP_LOGI(kTag, "Motor inputs GPIO1 and GPIO2 are low");

    // GPIO20 matches the LED connection used by the MicroPython boot script.
    ESP_ERROR_CHECK(gpio_set_level(kLed, 0));
    gpio_config_t led_pin = {};
    led_pin.pin_bit_mask = 1ULL << kLed;
    led_pin.mode = GPIO_MODE_OUTPUT;
    led_pin.pull_up_en = GPIO_PULLUP_DISABLE;
    led_pin.pull_down_en = GPIO_PULLDOWN_DISABLE;
    led_pin.intr_type = GPIO_INTR_DISABLE;
    ESP_ERROR_CHECK(gpio_config(&led_pin));
    ESP_ERROR_CHECK(gpio_set_level(kLed, 1));

    ledc_timer_config_t buzzer_timer = {};
    buzzer_timer.speed_mode = kBuzzerSpeedMode;
    buzzer_timer.timer_num = kBuzzerTimer;
    buzzer_timer.duty_resolution = LEDC_TIMER_10_BIT;
    buzzer_timer.freq_hz = 2000;
    buzzer_timer.clk_cfg = LEDC_AUTO_CLK;
    ESP_ERROR_CHECK(ledc_timer_config(&buzzer_timer));

    ledc_channel_config_t buzzer_channel = {};
    buzzer_channel.gpio_num = kBuzzer;
    buzzer_channel.speed_mode = kBuzzerSpeedMode;
    buzzer_channel.channel = kBuzzerChannel;
    buzzer_channel.timer_sel = kBuzzerTimer;
    buzzer_channel.duty = 512;  // 50% of 10-bit PWM
    buzzer_channel.hpoint = 0;
    ESP_ERROR_CHECK(ledc_channel_config(&buzzer_channel));

    ESP_LOGI(kTag, "LED on; buzzer on at 2 kHz");
    vTaskDelay(pdMS_TO_TICKS(1000));

    ESP_ERROR_CHECK(ledc_stop(kBuzzerSpeedMode, kBuzzerChannel, 0));
    ESP_ERROR_CHECK(gpio_set_level(kLed, 0));
    ESP_LOGI(kTag, "Boot complete; LED and buzzer off");
}
