# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.20

# compile C with /home/sjk/.espressif/tools/xtensa-esp32-elf/esp-2021r2-8.4.0/xtensa-esp32-elf/bin/xtensa-esp32-elf-gcc
C_DEFINES = -DHAVE_CONFIG_H -DMBEDTLS_CONFIG_FILE=\"mbedtls/esp_config.h\" -DUNITY_INCLUDE_CONFIG_H -DWITH_POSIX

C_INCLUDES = -I/home/sjk/laptopdir/course/embedded/esp32/greenh/comlib/server/build/config -I/home/sjk/esp/esp-idf-4.4/components/newlib/platform_include -I/home/sjk/esp/esp-idf-4.4/components/freertos/include -I/home/sjk/esp/esp-idf-4.4/components/freertos/include/esp_additions/freertos -I/home/sjk/esp/esp-idf-4.4/components/freertos/port/xtensa/include -I/home/sjk/esp/esp-idf-4.4/components/freertos/include/esp_additions -I/home/sjk/esp/esp-idf-4.4/components/esp_hw_support/include -I/home/sjk/esp/esp-idf-4.4/components/esp_hw_support/include/soc -I/home/sjk/esp/esp-idf-4.4/components/esp_hw_support/include/soc/esp32 -I/home/sjk/esp/esp-idf-4.4/components/esp_hw_support/port/esp32/. -I/home/sjk/esp/esp-idf-4.4/components/heap/include -I/home/sjk/esp/esp-idf-4.4/components/log/include -I/home/sjk/esp/esp-idf-4.4/components/lwip/include/apps -I/home/sjk/esp/esp-idf-4.4/components/lwip/include/apps/sntp -I/home/sjk/esp/esp-idf-4.4/components/lwip/lwip/src/include -I/home/sjk/esp/esp-idf-4.4/components/lwip/port/esp32/include -I/home/sjk/esp/esp-idf-4.4/components/lwip/port/esp32/include/arch -I/home/sjk/esp/esp-idf-4.4/components/soc/include -I/home/sjk/esp/esp-idf-4.4/components/soc/esp32/. -I/home/sjk/esp/esp-idf-4.4/components/soc/esp32/include -I/home/sjk/esp/esp-idf-4.4/components/hal/esp32/include -I/home/sjk/esp/esp-idf-4.4/components/hal/include -I/home/sjk/esp/esp-idf-4.4/components/hal/platform_port/include -I/home/sjk/esp/esp-idf-4.4/components/esp_rom/include -I/home/sjk/esp/esp-idf-4.4/components/esp_rom/include/esp32 -I/home/sjk/esp/esp-idf-4.4/components/esp_rom/esp32 -I/home/sjk/esp/esp-idf-4.4/components/esp_common/include -I/home/sjk/esp/esp-idf-4.4/components/esp_system/include -I/home/sjk/esp/esp-idf-4.4/components/esp_system/port/soc -I/home/sjk/esp/esp-idf-4.4/components/esp_system/port/public_compat -I/home/sjk/esp/esp-idf-4.4/components/esp32/include -I/home/sjk/esp/esp-idf-4.4/components/xtensa/include -I/home/sjk/esp/esp-idf-4.4/components/xtensa/esp32/include -I/home/sjk/esp/esp-idf-4.4/components/driver/include -I/home/sjk/esp/esp-idf-4.4/components/driver/esp32/include -I/home/sjk/esp/esp-idf-4.4/components/esp_pm/include -I/home/sjk/esp/esp-idf-4.4/components/esp_ringbuf/include -I/home/sjk/esp/esp-idf-4.4/components/efuse/include -I/home/sjk/esp/esp-idf-4.4/components/efuse/esp32/include -I/home/sjk/esp/esp-idf-4.4/components/vfs/include -I/home/sjk/esp/esp-idf-4.4/components/esp_wifi/include -I/home/sjk/esp/esp-idf-4.4/components/esp_event/include -I/home/sjk/esp/esp-idf-4.4/components/esp_netif/include -I/home/sjk/esp/esp-idf-4.4/components/esp_eth/include -I/home/sjk/esp/esp-idf-4.4/components/tcpip_adapter/include -I/home/sjk/esp/esp-idf-4.4/components/esp_phy/include -I/home/sjk/esp/esp-idf-4.4/components/esp_phy/esp32/include -I/home/sjk/esp/esp-idf-4.4/components/esp_ipc/include -I/home/sjk/esp/esp-idf-4.4/components/app_trace/include -I/home/sjk/esp/esp-idf-4.4/components/esp_timer/include -I/home/sjk/esp/esp-idf-4.4/components/mbedtls/port/include -I/home/sjk/esp/esp-idf-4.4/components/mbedtls/mbedtls/include -I/home/sjk/esp/esp-idf-4.4/components/mbedtls/esp_crt_bundle/include -I/home/sjk/esp/esp-idf-4.4/components/app_update/include -I/home/sjk/esp/esp-idf-4.4/components/spi_flash/include -I/home/sjk/esp/esp-idf-4.4/components/bootloader_support/include -I/home/sjk/esp/esp-idf-4.4/components/nvs_flash/include -I/home/sjk/esp/esp-idf-4.4/components/pthread/include -I/home/sjk/esp/esp-idf-4.4/components/esp_gdbstub/include -I/home/sjk/esp/esp-idf-4.4/components/esp_gdbstub/xtensa -I/home/sjk/esp/esp-idf-4.4/components/esp_gdbstub/esp32 -I/home/sjk/esp/esp-idf-4.4/components/espcoredump/include -I/home/sjk/esp/esp-idf-4.4/components/espcoredump/include/port/xtensa -I/home/sjk/esp/esp-idf-4.4/components/wpa_supplicant/include -I/home/sjk/esp/esp-idf-4.4/components/wpa_supplicant/port/include -I/home/sjk/esp/esp-idf-4.4/components/wpa_supplicant/esp_supplicant/include -I/home/sjk/esp/esp-idf-4.4/components/ieee802154/include -I/home/sjk/esp/esp-idf-4.4/components/console -I/home/sjk/esp/esp-idf-4.4/components/asio/asio/asio/include -I/home/sjk/esp/esp-idf-4.4/components/asio/port/include -I/home/sjk/esp/esp-idf-4.4/components/cbor/port/include -I/home/sjk/esp/esp-idf-4.4/components/unity/include -I/home/sjk/esp/esp-idf-4.4/components/unity/unity/src -I/home/sjk/esp/esp-idf-4.4/components/cmock/CMock/src -I/home/sjk/esp/esp-idf-4.4/components/coap/port/include -I/home/sjk/esp/esp-idf-4.4/components/coap/libcoap/include -I/home/sjk/esp/esp-idf-4.4/components/nghttp/port/include -I/home/sjk/esp/esp-idf-4.4/components/nghttp/nghttp2/lib/includes -I/home/sjk/esp/esp-idf-4.4/components/esp-tls -I/home/sjk/esp/esp-idf-4.4/components/esp-tls/esp-tls-crypto -I/home/sjk/esp/esp-idf-4.4/components/esp_adc_cal/include -I/home/sjk/esp/esp-idf-4.4/components/esp_hid/include -I/home/sjk/esp/esp-idf-4.4/components/tcp_transport/include -I/home/sjk/esp/esp-idf-4.4/components/esp_http_client/include -I/home/sjk/esp/esp-idf-4.4/components/esp_http_server/include -I/home/sjk/esp/esp-idf-4.4/components/esp_https_ota/include -I/home/sjk/esp/esp-idf-4.4/components/esp_lcd/include -I/home/sjk/esp/esp-idf-4.4/components/esp_lcd/interface -I/home/sjk/esp/esp-idf-4.4/components/protobuf-c/protobuf-c -I/home/sjk/esp/esp-idf-4.4/components/protocomm/include/common -I/home/sjk/esp/esp-idf-4.4/components/protocomm/include/security -I/home/sjk/esp/esp-idf-4.4/components/protocomm/include/transports -I/home/sjk/esp/esp-idf-4.4/components/mdns/include -I/home/sjk/esp/esp-idf-4.4/components/esp_local_ctrl/include -I/home/sjk/esp/esp-idf-4.4/components/sdmmc/include -I/home/sjk/esp/esp-idf-4.4/components/esp_serial_slave_link/include -I/home/sjk/esp/esp-idf-4.4/components/esp_websocket_client/include -I/home/sjk/esp/esp-idf-4.4/components/expat/expat/expat/lib -I/home/sjk/esp/esp-idf-4.4/components/expat/port/include -I/home/sjk/esp/esp-idf-4.4/components/wear_levelling/include -I/home/sjk/esp/esp-idf-4.4/components/fatfs/diskio -I/home/sjk/esp/esp-idf-4.4/components/fatfs/vfs -I/home/sjk/esp/esp-idf-4.4/components/fatfs/src -I/home/sjk/esp/esp-idf-4.4/components/freemodbus/common/include -I/home/sjk/esp/esp-idf-4.4/components/idf_test/include -I/home/sjk/esp/esp-idf-4.4/components/idf_test/include/esp32 -I/home/sjk/esp/esp-idf-4.4/components/jsmn/include -I/home/sjk/esp/esp-idf-4.4/components/json/cJSON -I/home/sjk/esp/esp-idf-4.4/components/libsodium/libsodium/src/libsodium/include -I/home/sjk/esp/esp-idf-4.4/components/libsodium/port_include -I/home/sjk/esp/esp-idf-4.4/components/mqtt/esp-mqtt/include -I/home/sjk/esp/esp-idf-4.4/components/openssl/include -I/home/sjk/esp/esp-idf-4.4/components/perfmon/include -I/home/sjk/esp/esp-idf-4.4/components/spiffs/include -I/home/sjk/esp/esp-idf-4.4/components/ulp/include -I/home/sjk/esp/esp-idf-4.4/components/wifi_provisioning/include

C_FLAGS = -mlongcalls -Wno-frame-address -ffunction-sections -fdata-sections -Wall -Werror=all -Wno-error=unused-function -Wno-error=unused-variable -Wno-error=deprecated-declarations -Wextra -Wno-unused-parameter -Wno-sign-compare -ggdb -Og -fmacro-prefix-map=/home/sjk/laptopdir/course/embedded/esp32/greenh/comlib/server=. -fmacro-prefix-map=/home/sjk/esp/esp-idf-4.4=IDF -fstrict-volatile-bitfields -Wno-error=unused-but-set-variable -fno-jump-tables -fno-tree-switch-conversion -std=gnu99 -Wno-old-style-declaration -D_GNU_SOURCE -DIDF_VER=\"v4.4-dirty\" -DESP_PLATFORM -D_POSIX_READER_WRITER_LOCKS

