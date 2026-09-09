# ESP32 minimal crash capture

ESP-IDF already captures the ESP32 equivalent of an ARM HardFault. Use the
ESP-IDF panic handler and ESP-IDF core dump support instead of replacing the
low-level exception path.

The panic handler captures the architecture-specific fault frame:

- Xtensa targets: PC, PS, A0..A15, SAR, EXCCAUSE, EXCVADDR, loop registers, and
  backtrace.
- RISC-V targets: MEPC, RA, SP, MSTATUS, MCAUSE, MTVAL, MHARTID, GPRs, and
  backtrace.

Fatal classes include CPU exceptions, interrupt watchdog, task watchdog, cache
errors, brownout, stack overflow, heap corruption, UBSAN checks, and assertions.

This example enables core dump to flash. No filesystem is involved: ESP-IDF
writes directly to a raw partition table entry of type `data`, subtype
`coredump`.

```csv
coredump, data, coredump,, 128K
```

At panic time, ESP-IDF serializes the crash state into that raw flash partition.
On the next boot, the application can read it with the user-mode coredump APIs:

```c
esp_reset_reason();
esp_core_dump_get_panic_reason(reason, sizeof(reason));
esp_core_dump_get_summary(summary);
esp_core_dump_image_get(&addr, &size);
esp_core_dump_image_erase();
```

This example exposes:

- `/api/crash`: JSON metadata: reset reason, panic reason, crashed task, PC, and
  coredump size.
- `/api/crash.bin`: raw ESP-IDF coredump image, suitable for offline decoding
  with the application ELF.

The raw coredump can include task stacks, TCBs, selected memory regions, and
therefore customer-private data. Production upload must treat it as sensitive
diagnostic material.

## Wizard integration notes

The Wizard ESP-IDF template should keep its existing `mongoose_init()` /
`mongoose_poll()` structure and register the crash endpoints as custom API
handlers. The relevant ESP-IDF pieces from this tutorial are:

```ini
CONFIG_ESP_COREDUMP_ENABLE_TO_FLASH=y
CONFIG_ESP_COREDUMP_DATA_FORMAT_ELF=y
CONFIG_ESP_COREDUMP_MAX_TASKS_NUM=16
CONFIG_ESP_COREDUMP_STACK_SIZE=2048
```

The ESP-IDF component needs `espcoredump` and `esp_partition`.

Wizard's current 4 MB OTA partition table leaves `0x10000` bytes after SPIFFS,
so a 64 KB coredump partition fits without changing the app or SPIFFS sizes.
The 128 KB partition used by this tutorial requires shrinking SPIFFS or one of
the app slots.

```csv
coredump, data, coredump,, 64K
```

The WiFi code is not materially related to crash capture. Apart from where the
credentials come from, the differences are retry policy, temporary event handler
lifetime, WPA2/PMF station config, and whether the password is logged. Wizard
can keep its existing WiFi code for this feature.
