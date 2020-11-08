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
#include <stdatomic.h>
#include <esp_event.h>
#include <esp_event_loop.h>

#include "task_js.h"
#include "http_server.h"

void app_main(void)
{
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    // 禁止 wifi 睡眠 (增加耗电)
    esp_wifi_set_ps(WIFI_PS_NONE);

	xTaskCreatePinnedToCore(&task_js_main, "task_js_main", 16*1024, NULL, 5, NULL, 0);
}