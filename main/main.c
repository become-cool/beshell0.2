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

#include "psram.h"
#include "js_main_loop.h"

#ifdef CONFIG_IDF_TARGET_ESP32S2
#include "tinyusb.h"
#include "tusb_cdc_acm.h"
static uint8_t buf[CONFIG_USB_CDC_RX_BUFSIZE + 1];
#endif

#include "sdkconfig.h"
#include "module_wifi.h"


static const char *TAG = "MAIN";

void blink() {
    gpio_config_t io_conf;
    // io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
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

#ifdef CONFIG_IDF_TARGET_ESP32S2
void tinyusb_cdc_rx_callback(int itf, cdcacm_event_t *event)
{
    /* initialization */
    size_t rx_size = 0;

    /* read */
    esp_err_t ret = tinyusb_cdcacm_read(itf, buf, CONFIG_USB_CDC_RX_BUFSIZE, &rx_size);
    if (ret == ESP_OK) {
        buf[rx_size] = '\0';
        ESP_LOGI(TAG, "Got data (%d bytes): %s", rx_size, buf);
    } else {
        ESP_LOGE(TAG, "Read error");
    }

    /* write back */
    tinyusb_cdcacm_write_queue(itf, (uint8_t*)">>", 2);
    tinyusb_cdcacm_write_queue(itf, buf, rx_size);
    tinyusb_cdcacm_write_flush(itf, 0);
}


void tinyusb_cdc_line_state_changed_callback(int itf, cdcacm_event_t *event)
{
    int dtr = event->line_state_changed_data.dtr;
    int rst = event->line_state_changed_data.rts;
    ESP_LOGI(TAG, "Line state changed! dtr:%d, rst:%d", dtr, rst);
}

void init_usb_cdc() {
    
    // ESP_LOGI(TAG, "USB initialization");
    // 
    // // Setting of descriptor. You can use descriptor_tinyusb and
    // // descriptor_str_tinyusb as a reference
    // tusb_desc_device_t descriptor = {
    //     .bLength = sizeof(descriptor),
    //     .bDescriptorType = TUSB_DESC_DEVICE,
    //     .bcdUSB = 0x0200, // USB version. 0x0200 means version 2.0
    //     .bDeviceClass = TUSB_CLASS_UNSPECIFIED,
    //     .bMaxPacketSize0 = CFG_TUD_ENDOINT0_SIZE,

    //     .idVendor = 0x303A,
    //     .idProduct = 0x3000,
    //     .bcdDevice = 0x0101, // Device FW version

    //     .iManufacturer = 0x01, // see string_descriptor[1] bellow
    //     .iProduct = 0x02,      // see string_descriptor[2] bellow
    //     .iSerialNumber = 0x03, // see string_descriptor[3] bellow

    //     .bNumConfigurations = 0x01
    // };

    // tusb_desc_strarray_device_t string_descriptor = {
    //     // array of pointer to string descriptors
    //     (char[]){0x09, 0x04},       // 0: is supported language is English (0x0409)
    //     "BECOME",                   // 1: Manufacturer
    //     "BECOME Host Part",                 // 2: Product
    //     "012345",                  // 3: Serials, should use chip ID
    // };

    // tinyusb_config_t tusb_cfg = {
    //     .descriptor = &descriptor,
    //     .string_descriptor = string_descriptor,
    //     .external_phy = false // In the most cases you need to use a `false` value
    // };

    ESP_LOGI(TAG, "USB initialization");
    tinyusb_config_t tusb_cfg = {}; // the configuration using default values
    ESP_ERROR_CHECK(tinyusb_driver_install(&tusb_cfg));

    tinyusb_config_cdcacm_t amc_cfg = {
        .usb_dev = TINYUSB_USBDEV_0,
        .cdc_port = TINYUSB_CDC_ACM_0,
        .rx_unread_buf_sz = 64,
        .callback_rx = &tinyusb_cdc_rx_callback, // the first way to register a callback
        .callback_rx_wanted_char = NULL,
        .callback_line_state_changed = NULL,
        .callback_line_coding_changed = NULL
    };

    ESP_ERROR_CHECK(tusb_cdc_acm_init(&amc_cfg));
    /* the second way to register a callback */
    ESP_ERROR_CHECK(tinyusb_cdcacm_register_callback(
                        TINYUSB_CDC_ACM_0,
                        CDC_EVENT_LINE_STATE_CHANGED,
                        &tinyusb_cdc_line_state_changed_callback));
    ESP_LOGI(TAG, "USB initialization DONE");
}
#endif




#include "nvs_flash.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include "module_wifi.h"

#include "driver/uart.h"




void app_main(void)
{
    //Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_ERROR_CHECK(esp_event_loop_create_default());

#ifdef CONFIG_IDF_TARGET_ESP32S2
    init_usb_cdc() ;
    js_main_loop(NULL) ;
#else

    printf("Heap  (free/total): %d/%d\n", getHeapUsed(),getHeapTotal());
    printf("DMA   (free/total): %d/%d\n", getDMAUsed(),getDMATotal());
    printf("PSRAM (free/total): %d/%d\n", getPsramUsed(),getPsramTotal());

    // 优先级 tskIDLE_PRIORITY 可以避免触发看门狗
    xTaskCreatePinnedToCore(&js_main_loop, "js_main_loop", 20*1024, NULL, tskIDLE_PRIORITY, NULL, 0);
#endif

}
