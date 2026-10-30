set(CMAKE_SYSTEM_NAME               Generic)
set(CMAKE_SYSTEM_PROCESSOR          arm)

set(CMAKE_C_COMPILER_ID Clang)
set(CMAKE_CXX_COMPILER_ID Clang)

# Some default llvm settings
set(TOOLCHAIN_PREFIX                starm-)

set(CMAKE_C_COMPILER                ${TOOLCHAIN_PREFIX}clang)
set(CMAKE_ASM_COMPILER              ${CMAKE_C_COMPILER})
set(CMAKE_CXX_COMPILER              ${TOOLCHAIN_PREFIX}clang++)
set(CMAKE_LINKER                    ${TOOLCHAIN_PREFIX}clang)
set(CMAKE_OBJCOPY                   ${TOOLCHAIN_PREFIX}objcopy)
set(CMAKE_SIZE                      ${TOOLCHAIN_PREFIX}size)

set(CMAKE_EXECUTABLE_SUFFIX_ASM     ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_C       ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_CXX     ".elf")

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# STARM_TOOLCHAIN_CONFIG allows you to choose the toolchain configuration.
# Possible values are:
#  "STARM_HYBRID"   : Hybrid configuration using starm-clang Assemler and Compiler and GNU Linker
#  "STARM_NEWLIB"   : starm-clang toolchain with NEWLIB C library
#  "STARM_PICOLIBC" : starm-clang toolchain with PICOLIBC C library
set(STARM_TOOLCHAIN_CONFIG "STARM_PICOLIBC")

if(STARM_TOOLCHAIN_CONFIG STREQUAL "STARM_HYBRID")
  set(TOOLCHAIN_MULTILIBS "--multi-lib-config=\"$ENV{CLANG_GCC_CMSIS_COMPILER}/multilib.gnu_tools_for_stm32.yaml\" --gcc-toolchain=\"$ENV{GCC_TOOLCHAIN_ROOT}/..\"")
elseif (STARM_TOOLCHAIN_CONFIG STREQUAL "STARM_NEWLIB")
  set(TOOLCHAIN_MULTILIBS "--config=newlib.cfg")
endif()

# MCU specific flags
set(TARGET_FLAGS "-mcpu=cortex-m7 -mfpu=fpv5-d16 -mfloat-abi=hard ${TOOLCHAIN_MULTILIBS}")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${TARGET_FLAGS}")
set(CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS} -x assembler-with-cpp -MP")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -fdata-sections -ffunction-sections -ftls-model=local-exec -fstack-usage")

set(CMAKE_C_FLAGS_DEBUG "-Og -g3")
set(CMAKE_C_FLAGS_RELEASE "-Oz -g0")
set(CMAKE_CXX_FLAGS_DEBUG "-Og -g3")
set(CMAKE_CXX_FLAGS_RELEASE "-Oz -g0")

set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -fno-rtti -fno-exceptions -fno-threadsafe-statics")

set(CMAKE_EXE_LINKER_FLAGS "${TARGET_FLAGS}")

if (STARM_TOOLCHAIN_CONFIG STREQUAL "STARM_HYBRID")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --gcc-specs=nano.specs")
  set(TOOLCHAIN_LINK_LIBRARIES "m")
elseif(STARM_TOOLCHAIN_CONFIG STREQUAL "STARM_NEWLIB")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lcrt0-nosys")
elseif(STARM_TOOLCHAIN_CONFIG STREQUAL "STARM_PICOLIBC")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lcrt0-hosted -z norelro")

endif()

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T \"${CMAKE_SOURCE_DIR}/STM32H723XG_FLASH.ld\"")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Map=${CMAKE_PROJECT_NAME}.map -Wl,--gc-sections")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -z noexecstack")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--print-memory-usage ")
