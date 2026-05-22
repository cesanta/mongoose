if(NOT EXISTS mbed/.git)
  execute_process(COMMAND git clone --depth 1 --no-checkout -b extrapatches-6.17.0 https://github.com/arduino/mbed-os mbed)
endif()

execute_process(COMMAND git -C mbed sparse-checkout set targets/TARGET_STM/TARGET_STM32H7/TARGET_STM32H747xI/TARGET_PORTENTA_H7/COMPONENT_WHD/resources)
execute_process(COMMAND git -C mbed checkout)
file(TOUCH wiced_resource.h)
