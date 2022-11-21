#include "esp32_perfmon.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_freertos_hooks.h"
#include "sdkconfig.h"

#include "esp_log.h"
static const char *TAG = "perfmon";

static uint64_t idle0Calls = 0;
static uint64_t idle1Calls = 0;

static uint64_t lastIdle0Calls = 0;
static uint64_t lastIdle1Calls = 0;

uint8_t cpu0 = 0 ;
uint8_t cpu1 = 0 ;

// ESP32
#if defined(CONFIG_ESP32_DEFAULT_CPU_FREQ_240)
static const uint64_t MaxIdleCalls = 1855000;
#elif defined(CONFIG_ESP32_DEFAULT_CPU_FREQ_160)
static const uint64_t MaxIdleCalls = 1233100;

// ESP32S3
#elif defined(CONFIG_ESP32S3_DEFAULT_CPU_FREQ_240)
static const uint64_t MaxIdleCalls = 1855000;
#elif defined(CONFIG_ESP32S3_DEFAULT_CPU_FREQ_160)
static const uint64_t MaxIdleCalls = 1233100;
#else

#error "Unsupported CPU frequency"
#endif

uint8_t cpu0_usage() {
	return cpu0 ;
}
uint8_t cpu1_usage() {
	return cpu1 ;
}
uint64_t cpu0_idle_ticks() {
	return lastIdle0Calls ;
}
uint64_t cpu1_idle_ticks() {
	return lastIdle1Calls ;
}

static bool idle_task_0()
{
	idle0Calls += 1;
	return false;
}

static bool idle_task_1()
{
	idle1Calls += 1;
	return false;
}

static void perfmon_task(void *args)
{
	while (1)
	{
		lastIdle0Calls = idle0Calls;
		lastIdle1Calls = idle1Calls;

		idle0Calls = 0;
		idle1Calls = 0;

		cpu0 = 100 -  100 * lastIdle0Calls / MaxIdleCalls;
		cpu1 = 100 - 100 * lastIdle1Calls / MaxIdleCalls;

		// ESP_LOGI(TAG, "Core 0 at %d%%", cpu0);
		// ESP_LOGI(TAG, "Core 1 at %d%%", cpu1);
		// TODO configurable delay
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
	vTaskDelete(NULL);
}

esp_err_t perfmon_start()
{
	// return ESP_OK ;
	ESP_ERROR_CHECK(esp_register_freertos_idle_hook_for_cpu(idle_task_0, 0));
	ESP_ERROR_CHECK(esp_register_freertos_idle_hook_for_cpu(idle_task_1, 1));
	// TODO calculate optimal stack size
	xTaskCreate(perfmon_task, "perfmon", 1024, NULL, 1, NULL);
	return ESP_OK;
}
