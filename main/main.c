#include "main.h"

void app_main(void)
{
    printf("Hello world!\n");

    /* Print chip information */
    esp_chip_info_t chip_info;
    uint32_t flash_size;
    esp_chip_info(&chip_info);
    printf("This is %s chip with %d CPU core(s), %s%s%s%s, ",
           CONFIG_IDF_TARGET,
           chip_info.cores,
           (chip_info.features & CHIP_FEATURE_WIFI_BGN) ? "WiFi/" : "",
           (chip_info.features & CHIP_FEATURE_BT) ? "BT" : "",
           (chip_info.features & CHIP_FEATURE_BLE) ? "BLE" : "",
           (chip_info.features & CHIP_FEATURE_IEEE802154) ? ", 802.15.4 (Zigbee/Thread)" : "");

    unsigned major_rev = chip_info.revision / 100;
    unsigned minor_rev = chip_info.revision % 100;
    printf("silicon revision v%d.%d, ", major_rev, minor_rev);
    if (esp_flash_get_size(NULL, &flash_size) != ESP_OK)
    {
        printf("Get flash size failed");
        return;
    }

    printf("%" PRIu32 "MB %s flash\n", flash_size / (uint32_t)(1024 * 1024),
           (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

    printf("Minimum free heap size: %" PRIu32 " bytes\n", esp_get_minimum_free_heap_size());

    // ESP_ERROR_CHECK(SysRunningLedInit());
    ESP_ERROR_CHECK(I2cImuInit());
    ESP_ERROR_CHECK(I2cTofInit());
    ESP_ERROR_CHECK(uartgpsdevInit());

    xTaskCreate(&SysRunningLedTask, "SysRunningLedTask", 1024 * 2, NULL, 10, NULL);
    xTaskCreate(&ImuTask, "IMU", 1024 * 10, NULL, 5, NULL);
    xTaskCreate(&TofTask, "TOF", 1024 * 12, NULL, 6, NULL);
    xTaskCreate(&GpsTask, "GPS", 1024 * 10, NULL, 7, NULL);
}
