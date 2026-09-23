#include "driver/gpio.h"
#include "driver/ledc.h"
#include "esp_check.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

namespace {
constexpr gpio_num_t kMotorIn1 = GPIO_NUM_1;
constexpr gpio_num_t kMotorIn2 = GPIO_NUM_42;
constexpr gpio_num_t kLed = GPIO_NUM_21;
constexpr gpio_num_t kBuzzer = GPIO_NUM_41;
constexpr ledc_mode_t kBuzzerSpeedMode = LEDC_LOW_SPEED_MODE;
constexpr ledc_timer_t kBuzzerTimer = LEDC_TIMER_0;
constexpr ledc_channel_t kBuzzerChannel = LEDC_CHANNEL_0;
constexpr ledc_timer_t kMotorTimer = LEDC_TIMER_1;
constexpr ledc_channel_t kMotorIn1Channel = LEDC_CHANNEL_1;
constexpr ledc_channel_t kMotorIn2Channel = LEDC_CHANNEL_2;
constexpr uint32_t kMotorDuty = 512;  // 50% of 10-bit PWM
constexpr char kTag[] = "boot";

void set_motor_duty(ledc_channel_t channel, uint32_t duty) {
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_LOW_SPEED_MODE, channel, duty));
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_LOW_SPEED_MODE, channel));
}

void stop_motor() {
    set_motor_duty(kMotorIn1Channel, 0);
    set_motor_duty(kMotorIn2Channel, 0);
}
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

    ESP_LOGI(kTag, "Motor inputs GPIO1 and GPIO42 are low");

    // The LED is connected to GPIO21; GPIO20 is reserved for native USB.
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

    ledc_timer_config_t motor_timer = {};
    motor_timer.speed_mode = LEDC_LOW_SPEED_MODE;
    motor_timer.timer_num = kMotorTimer;
    motor_timer.duty_resolution = LEDC_TIMER_10_BIT;
    motor_timer.freq_hz = 2000;
    motor_timer.clk_cfg = LEDC_AUTO_CLK;
    ESP_ERROR_CHECK(ledc_timer_config(&motor_timer));

    ledc_channel_config_t motor_in1 = {};
    motor_in1.gpio_num = kMotorIn1;
    motor_in1.speed_mode = LEDC_LOW_SPEED_MODE;
    motor_in1.channel = kMotorIn1Channel;
    motor_in1.timer_sel = kMotorTimer;
    motor_in1.duty = 0;
    motor_in1.hpoint = 0;
    ESP_ERROR_CHECK(ledc_channel_config(&motor_in1));

    ledc_channel_config_t motor_in2 = motor_in1;
    motor_in2.gpio_num = kMotorIn2;
    motor_in2.channel = kMotorIn2Channel;
    ESP_ERROR_CHECK(ledc_channel_config(&motor_in2));

    ESP_LOGI(kTag, "Motor stopped");
    vTaskDelay(pdMS_TO_TICKS(2000));

    ESP_LOGI(kTag, "Motor forward at 50%%");
    set_motor_duty(kMotorIn1Channel, kMotorDuty);
    vTaskDelay(pdMS_TO_TICKS(1000));
    stop_motor();

    ESP_LOGI(kTag, "Motor stopped");
    vTaskDelay(pdMS_TO_TICKS(1000));

    ESP_LOGI(kTag, "Motor reverse at 50%%");
    set_motor_duty(kMotorIn2Channel, kMotorDuty);
    vTaskDelay(pdMS_TO_TICKS(1000));
    stop_motor();

    ESP_LOGI(kTag, "Motor test complete; motor stopped");
}
