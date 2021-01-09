/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#define CONFIG_LOG_DEFAULT_LEVEL ESP_LOG_VERBOSE

#include <stdio.h>
#include <sys/fcntl.h>
#include <sys/errno.h>
#include <sys/unistd.h>
#include <sys/select.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_vfs.h"
#include "esp_vfs_dev.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "driver/gpio.h"
#include <stdatomic.h>
#include <esp_event.h>
#include <esp_event_loop.h>

#include "task_js.h"
#include "http_server.h"



void blink() {
    gpio_config_t io_conf;
	io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
	io_conf.mode = GPIO_MODE_OUTPUT;
	io_conf.pull_down_en = 0;
	io_conf.pull_up_en = 0;
    io_conf.pin_bit_mask = (1ULL << 16) | (1ULL << 17) | (1ULL << 18) ;
    gpio_config(&io_conf);

    gpio_set_level(16, 0);
    gpio_set_level(17, 1);
    gpio_set_level(18, 1);

    vTaskDelay(pdMS_TO_TICKS(100));
    gpio_set_level(16, 1);
    gpio_set_level(17, 0);
    gpio_set_level(18, 1);

    vTaskDelay(pdMS_TO_TICKS(100));
    gpio_set_level(16, 1);
    gpio_set_level(17, 1);
    gpio_set_level(18, 0);

    vTaskDelay(pdMS_TO_TICKS(100));
    gpio_set_level(16, 0);
    gpio_set_level(17, 1);
    gpio_set_level(18, 1);

    vTaskDelay(pdMS_TO_TICKS(100));
    gpio_set_level(16, 1);
    gpio_set_level(17, 0);
    gpio_set_level(18, 1);

    vTaskDelay(pdMS_TO_TICKS(100));
    gpio_set_level(16, 1);
    gpio_set_level(17, 1);
    gpio_set_level(18, 0);
}


void app_main(void)
{
    blink() ;

    ESP_ERROR_CHECK(esp_event_loop_create_default());

	xTaskCreatePinnedToCore(&task_js_main, "task_js_main", 16*1024, NULL, 5, NULL, 0);
}