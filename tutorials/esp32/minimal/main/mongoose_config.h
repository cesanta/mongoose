#define MG_ARCH MG_ARCH_ESP32

// Crash reports and mem usage stats
// Make sure to add to sdkconfig.defaults the following:
//   CONFIG_ESP_COREDUMP_ENABLE_TO_FLASH=y
//   CONFIG_ESP_COREDUMP_DATA_FORMAT_ELF=y
//   CONFIG_ESP_COREDUMP_MAX_TASKS_NUM=16
//   CONFIG_ESP_COREDUMP_STACK_SIZE=2048
//   CONFIG_ESP_MAIN_TASK_STACK_SIZE=8192
#define MG_ENABLE_HEALTH 1

// mdash.net device management support, https://mongoose.ws/docs/guides/mdash/
#define MG_ENABLE_MDASH 0
#define MG_MDASH_KEY "..."
#define MG_FIRMWARE_VERSION "esp32_min.1.0.0"
#define MG_TLS MG_TLS_BUILTIN
