#include "power.h"

#include "freertos/FreeRTOS.h"
#include "esp_system.h"
#include "esp_event.h"
#include "driver/rtc_io.h"
#include <driver/adc.h>
#include "esp_adc_cal.h"
#include "gamepad.h"
#include "pin_definitions.h"

static esp_adc_cal_characteristics_t characteristics;
static bool input_battery_initialized = false;
static float adc_value = 0.0f;
static float forced_adc_value = 0.0f;
static bool battery_monitor_enabled = true;

static bool system_initialized = false;

void system_sleep()
{
    printf("%s: Entered.\n", __func__);

    // Wait for button release
    input_gamepad_state joystick;
    gamepad_read(&joystick);

    while (joystick.values[GAMEPAD_INPUT_MENU])
    {
        vTaskDelay(1);
        gamepad_read(&joystick);
    }

    // Configure button to wake
    printf("%s: Configuring deep sleep.\n", __func__);
#if 1
    esp_err_t err = esp_sleep_enable_ext0_wakeup(MENU, 0);
#else
    const int ext_wakeup_pin_1 = ODROID_GAMEPAD_IO_MENU;
    const uint64_t ext_wakeup_pin_1_mask = 1ULL << ext_wakeup_pin_1;

    esp_err_t err = esp_sleep_enable_ext1_wakeup(ext_wakeup_pin_1_mask, ESP_EXT1_WAKEUP_ALL_LOW);
#endif
    if (err != ESP_OK)
    {
        printf("%s: esp_sleep_enable_ext0_wakeup failed.\n", __func__);
        abort();
    }
    /*
    err = rtc_gpio_pullup_en(MENU);
    if (err != ESP_OK)
    {
        printf("%s: rtc_gpio_pullup_en failed.\n", __func__);
        abort();
    }
*/
    // Isolate GPIO12 pin from external circuits. This is needed for modules
    // which have an external pull-up resistor on GPIO12 (such as ESP32-WROVER)
    // to minimize current consumption.
    //rtc_gpio_isolate(GPIO_NUM_12);
#if 1
    //rtc_gpio_isolate(GPIO_NUM_34);
    //rtc_gpio_isolate(GPIO_NUM_35);
    //rtc_gpio_isolate(GPIO_NUM_0);
    //rtc_gpio_isolate(GPIO_NUM_39);
    //rtc_gpio_isolate(GPIO_NUM_14);
#endif

    // Sleep
    //esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_ON);

    vTaskDelay(100);
    esp_deep_sleep_start();
}

void esplay_system_init()
{
    rtc_gpio_deinit(MENU);
    //rtc_gpio_deinit(GPIO_NUM_14);

    system_initialized = true;
}

static void print_char_val_type(esp_adc_cal_value_t val_type)
{
    if (val_type == ESP_ADC_CAL_VAL_EFUSE_TP)
    {
        printf("ADC: Characterized using Two Point Value\n");
    }
    else if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF)
    {
        printf("ADC: Characterized using eFuse Vref\n");
    }
    else
    {
        printf("ADC: Characterized using Default Vref\n");
    }
}

void system_led_set(int state)
{
    gpio_set_level(LED1, state);
}

charging_state getChargeStatus()
{
    if(!gpio_get_level(USB_PLUG_PIN))
        return NO_CHRG;
    else
    {
        if(!gpio_get_level(CHRG_STATE_PIN))
            return CHARGING;
        else
            return FULL_CHARGED;
    }
}

static void battery_monitor_task()
{
    bool led_state = false;
    charging_state chrg;
    int fullCtr=0;
	//The LiIon charger sometimes goes back from 'full' to 'charging', which is
	//confusing to the end user. This variable becomes true if the LiIon has indicated 'full'
	//for a while, and it being true causes the 'full' icon to always show.
	int fixFull=0;
    while (true)
    {
        if (battery_monitor_enabled)
        {
            battery_state battery;
            battery_level_read(&battery);

            if (battery.percentage < 2)
            {
                led_state = !led_state;
                system_led_set(led_state);
            }
            else if (led_state)
            {
                led_state = 0;
                system_led_set(led_state);
            }
            else
            {
                chrg = getChargeStatus();
                if (chrg == FULL_CHARGED || fixFull)
                {
                    fullCtr++;
                    system_led_set(0);
                }

                if (chrg == CHARGING)
                {
                    fullCtr = 0;
                    system_led_set(1);
                }

                if (fullCtr == 32)
                {
                    fixFull = 1;
                }

                if (chrg == NO_CHRG)
                {
                    system_led_set(0);
                }
            }
        }

        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

#define DEFAULT_VREF 1100
void battery_level_init()
{
    PIN_FUNC_SELECT(GPIO_PIN_MUX_REG[LED1], PIN_FUNC_GPIO);
    gpio_set_direction(LED1, GPIO_MODE_OUTPUT);
    gpio_set_direction(USB_PLUG_PIN, GPIO_MODE_INPUT);
    gpio_set_pull_mode(USB_PLUG_PIN, GPIO_PULLUP_ONLY);
    gpio_set_direction(CHRG_STATE_PIN, GPIO_MODE_INPUT);
    gpio_set_pull_mode(CHRG_STATE_PIN, GPIO_PULLUP_ONLY);
    adc1_config_width(ADC_WIDTH_12Bit);
    adc1_config_channel_atten(ADC_PIN, ADC_ATTEN_11db);

    //int vref_value = odroid_settings_VRef_get();
    //esp_adc_cal_get_characteristics(vref_value, ADC_ATTEN_11db, ADC_WIDTH_12Bit, &characteristics);

    //Characterize ADC
    //adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_value_t val_type = esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_11db, ADC_WIDTH_BIT_12, DEFAULT_VREF, &characteristics);
    print_char_val_type(val_type);

    input_battery_initialized = true;
    battery_monitor_enabled_set(true);
    xTaskCreatePinnedToCore(&battery_monitor_task, "battery_monitor", 1024, NULL, 5, NULL, 1);
}

void battery_level_read(battery_state *out_state)
{
    if (!input_battery_initialized)
    {
        printf("battery_level_read: not initilized.\n");
        abort();
    }

    const int sampleCount = 8;

    float adcSample = 0.0f;
    for (int i = 0; i < sampleCount; ++i)
    {
        //adcSample += adc1_to_voltage(ADC1_CHANNEL_0, &characteristics) * 0.001f;
        adcSample += esp_adc_cal_raw_to_voltage(adc1_get_raw(ADC_PIN), &characteristics) * 0.001f;
    }
    adcSample /= sampleCount;

    if (adc_value == 0.0f)
    {
        adc_value = adcSample;
    }
    else
    {
        adc_value += adcSample;
        adc_value /= 2.0f;
    }

    // Vo = (Vs * R2) / (R1 + R2)
    // Vs = Vo / R2 * (R1 + R2)
    const float R1 = 100000;
    const float R2 = 100000;
    const float Vo = adc_value;
    const float Vs = (forced_adc_value > 0.0f) ? (forced_adc_value) : (Vo / R2 * (R1 + R2));

    const float FullVoltage = 4.1f;
    const float EmptyVoltage = 3.4f;

    out_state->millivolts = (int)(Vs * 1000);
    out_state->percentage = (int)((Vs - EmptyVoltage) / (FullVoltage - EmptyVoltage) * 100.0f);
    if (out_state->percentage > 100)
        out_state->percentage = 100;
    if (out_state->percentage < 0)
        out_state->percentage = 0;
}

void battery_level_force_voltage(float volts)
{
    forced_adc_value = volts;
}

void battery_monitor_enabled_set(int value)
{
    battery_monitor_enabled = value;
}