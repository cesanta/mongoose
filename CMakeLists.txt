cmake_minimum_required(VERSION 3.20)
#Note: The following 3 variables should be set before project() call
set(CMAKE_TOOLCHAIN_FILE "/opt/wasi-sdk/share/cmake/wasi-sdk.cmake")#Set paths to compilers
set(WASI_SDK_PREFIX "/opt/wasi-sdk") #Used by toolchain file(wasi-sdk.cmake)
set(CMAKE_SYSROOT "/opt/wasi-sdk/share/wasi-sysroot/") #Root directory used for locating headers and libraries

project(kms-modules)


set(CMAKE_CXX_STANDARD 20)

set(default_build_type "Debug")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
      STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

if(NOT DEFINED WASI_SDK_PATH)
    set(WASI_SDK_PATH "/opt/wasi-sdk")
endif()

if(NOT DEFINED WOLFSSL_ROOT)
    set(WOLFSSL_ROOT "/opt/wolfssl")
endif()

if(NOT DEFINED WASM_WOLFSSL_LIB)
    set(WASM_WOLFSSL_LIB "${WOLFSSL_ROOT}/IDE/Wasm")
endif()

if(NOT DEFINED WAMR_PATH)
    set(WAMR_PATH "/wasm-micro-runtime")
endif()

if(NOT DEFINED HOME_ROOT)
    set(HOME_ROOT "/usr/src/app")
endif()

file(GLOB MONGOOSE_SRCS mongoose.c ${WAMR_PATH}/core/iwasm/libraries/lib-socket/src/wasi/wasi_socket_ext.c)

include_directories(src)
include_directories(mip)



set(WARN_FLAGS "-pedantic -W -Wall -Werror -Wshadow -Wdouble-promotion -fno-common -Wconversion -Wundef -Wno-deprecated")
set(OPT_FLAGS "-O3 -g3")
set(ASAN_FLAGS "-fsanitize=address,undefined,alignment -fno-sanitize-recover=all -fno-omit-frame-pointer -fno-common")
set(VC_FLAGS "/nologo /W3 /O2 /MD /I. $(DEFS) $(TFLAGS)")
if(NOT DEFINED SSL)
    set(SSL "WOLFSSL") 
endif()

if(NOT DEFINED ASAN_OPTIONS_FLAG)
    set(ASAN_OPTIONS_FLAG "detect_leaks=1")
endif()

if(NOT DEFINED COMMON_CFLAGS)
    set(COMMON_CFLAGS "${C_WARN} ${WARN} ${INCS} ${DEFS} ${TFLAGS}")
endif()

if(NOT DEFINED CFLAGS)
    set(CFLAGS "${OPTS} ${ASAN} ${COMMON_CFLAGS}")
endif()

if(NOT DEFINED VALGRIND_CFLAGS)
    set(VALGRIND_CFLAGS "${OPTS} ${COMMON_CFLAGS}")
endif()

if(NOT DEFINED VALGRIND_RUN)
    set(VALGRIND_RUN "valgrind --tool=memcheck --gen-suppressions=all --leak-check=full --show-leak-kinds=all --leak-resolution=high --track-origins=yes --error-exitcode=1 --exit-on-first-error=yes")
endif()

if(SSL STREQUAL "MBEDTLS")
    if(NOT DEFINED MBEDTLS)
        set(MBEDTLS "/usr/local")
    endif()
    add_definitions(-DMG_ENABLE_MBEDTLS=1)
    include_directories(${MBEDTLS}/include /usr/include)
    link_directories(${MBEDTLS}/lib)
    link_libraries(mbedtls mbedcrypto mbedx509)
endif()

if(SSL STREQUAL "OPENSSL")
    if(NOT DEFINED WOLFSSL)
        set(WOLFSSL "/usr/local")
    endif()
    add_definitions(-DMG_ENABLE_OPENSSL=1)
    include_directories(${OPENSSL}/include)
    link_directories(${OPENSSL}/lib)
    link_libraries(ssl crypto)
endif()

if(SSL STREQUAL "WOLFSSL")
    if(NOT DEFINED WOLFSSL)
        set(WOLFSSL "/usr/local")
    endif()
    add_definitions(-DMG_ENABLE_WOLFSSL=1)
    add_definitions(-DMG_ENABLE_WOLFSSL_DEBUG)
    include_directories(${WOLFSSL}/include)
    link_directories(${WOLFSSL}/lib)
    link_libraries(wolfssl)
    # if(MG_ENABLE_WOLFSSL_DEBUG)
    #     add_definitions(MG_ENABLE_WOLFSSL_DEBUG)
    # endif()
endif()

add_definitions(-DMG_MAX_HTTP_HEADERS=7)
add_definitions(-DMG_ENABLE_LINES)
add_definitions(-DMG_ENABLE_SSI=1)
add_definitions(-DMG_ENABLE_PACKED_FS=1)
add_definitions(-DMG_ENABLE_IPV6=0)
add_definitions(-DWOLFSSL_WASM=1)
add_definitions(-DSQLITE_OS_OTHER)
add_definitions(-DSQLITE_ENABLE_MEMSYS3)

# -Wall \
# -Wextra \
# -Werror \
# -pedantic \
# -Wno-deprecated \

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} --target=wasm32-wasi \
                            -Wl,--export=malloc -Wl,--export=free \
                            -Wl,--export=realloc \
                            -Wl,--export=__heap_base \
                            -Wl,--export=__data_end \
                            -Wl,--initial-memory=10485760 \
                            -z stack-size=8388608 \
                            -Wl,--strip-all \
                            ${CFLAGS} \
                            ${LDFLAGS} \
                            ")

set (CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_DEBUG} -fno-omit-frame-pointer -DNDEBUG -O3")

# -g \
# -Wall \
# -Wextra \
# -Werror \
# -pedantic \
# -fpermissive\

set(CMAKE_CXX_FLAGS_DEBUG "${CFLAGS} ${CMAKE_CXX_FLAGS_DEBUG} \
                                -Wno-unused-command-line-argument \
                                -Wno-deprecated -isysroot \
                                -std=c++2b -O3 -g3 \
                                -Wl,--export=malloc -Wl,--export=free \
                                -Wl,--export=realloc \
                                -Wl,--export=__heap_base \
                                -Wl,--export=__data_end \
                                -Wl,--initial-memory=10485760 \
                                -z stack-size=8388608 \
                                -Wl,--allow-undefined \
                                -fwasm-exceptions \
                                -fno-exceptions \
                                -Wno-vla-extension \
                                --sysroot=${WASI_SDK_PATH}/share/wasi-sysroot/ \
                                --target=wasm32-wasi \
                                -Wl,--strip-all")


set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -DNDEBUG -O3")

##################################################################################


# if(NOT DEFINED SKIP_CPPCHECK OR NOT SKIP_CPPCHECK)
#   message("-- SKIP_CPPCHECK: False")
#   find_program(CMAKE_CPPCHECK NAMES cppcheck)
#       add_custom_target(cppcheck-analysis ALL
#           COMMAND ${CMAKE_CPPCHECK} ${CMAKE_CURRENT_SOURCE_DIR} --enable=all --error-exitcode=1 -q --inline-suppr --suppressions-list=${CMAKE_CURRENT_SOURCE_DIR}/CppCheckSuppressions.txt -i${CMAKE_CURRENT_SOURCE_DIR}/build -i${CMAKE_CURRENT_SOURCE_DIR}/include
#           VERBATIM
#           COMMENT "Running Cppcheck")
# else()
#   message("-- SKIP_CPPCHECK: True")
# endif()

set(WAMRC_SIZE_LEVEL 1)
set(WAMRC_OPT_LEVEL 3)
set(WAMRC "${WAMR_PATH}/wamr-compiler/build/wamrc")
# add --enable-dump-call-stack to WAMRC_PARAMS to enable debug stack trace
set(WAMRC_PARAMS --size-level=${WAMRC_SIZE_LEVEL} --opt-level=${WAMRC_OPT_LEVEL} -sgx)

# include_directories(${WOLFSSL_ROOT}/wolfcrypt ${WOLFSSL_ROOT} /opt  ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/../crypto ${CMAKE_CURRENT_SOURCE_DIR}/../../include /opt/intel/sgxsdk/include/ ../../Enclave ${WAMR_PATH}/core/iwasm/libraries/lib-socket/inc/)

add_definitions(-DWOLFSSL_WASM -DWOLFSSL_AES_DIRECT -DHAVE_AES_KEYWRAP -DDISABLE_TRACE -DHAVE_WASI_SOCKET -DJSON_HAS_FILESYSTEM=0 -DWAMRC_SIZE_LEVEL=1 -DWAMRC_OPT_LEVEL=3)

set_source_files_properties(${WAMR_PATH}/core/iwasm/libraries/lib-socket/src/wasi/wasi_socket_ext.c PROPERTIES LANGUAGE CXX )
set_source_files_properties(${WAMR_PATH}/core/iwasm/libraries/lib-socket/src/wasi/wasi_socket_ext.c PROPERTIES COMPILE_FLAGS "-Wno-missing-field-initializers -Wno-unused-variable -Wno-unused-parameter -Wno-sign-compare -Wno-unused-function" )
# add_library(wasi_socket STATIC )
set_source_files_properties(mongoose.c PROPERTIES LANGUAGE CXX )

add_library(mongoose_lib STATIC ${MONGOOSE_SRCS})
target_include_directories(mongoose_lib PUBLIC ${WOLFSSL_ROOT}/wolfcrypt ${WOLFSSL_ROOT} /opt ${WOLFSSL_ROOT}/wolfssl ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/../crypto ${CMAKE_CURRENT_SOURCE_DIR}/../../include /opt/intel/sgxsdk/include/ ../../Enclave ${WAMR_PATH}/core/iwasm/libraries/lib-socket/inc/ ${CMAKE_CURRENT_SOURCE_DIR}/src)


link_directories(${WASM_WOLFSSL_LIB})
link_libraries(wolfssl)


include_directories(${CMAKE_SOURCE_DIR}/include ${WOLFSSL_ROOT} ${WOLFSSL_ROOT}/wolfssl ${WAMR_PATH}/core/iwasm/libraries/lib-socket/inc/ ${WAMR_PATH}/core/iwasm/libraries/lib-rats)

include_directories(./)



file(GLOB CORE_SRC testdns.cpp)
add_library(kms_core_lib ${CORE_SRC})
target_link_libraries( kms_core_lib mongoose_lib)

add_executable(testdns.wasm testdns.cpp )
target_link_libraries(testdns.wasm PRIVATE kms_core_lib)


