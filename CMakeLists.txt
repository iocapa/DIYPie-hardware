cmake_minimum_required(VERSION 3.15)

# Print size
function(target_print_size target)
    add_custom_command(TARGET ${target} POST_BUILD COMMAND ${CMAKE_SIZE} -A "$<TARGET_FILE:${target}>")
endfunction()

# Add linker script
function(target_linker_script target script)
    target_link_options(${target} PRIVATE "-T${script}")
    get_target_property(_curr_link_deps ${target} LINK_DEPENDS)
    string(APPEND _curr_link_deps "${script}")
    set_target_properties(${target} PROPERTIES LINK_DEPENDS "${script}")
endfunction()

# Could be moved externally in case of multiple toolchains
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/arm-none-eabi-gcc.cmake)

# Project
set(PROJECT_NAME "DIYPie-firmware")
project(${PROJECT_NAME} LANGUAGES C)

# Paths
set(FREERTOS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS-Kernel)
set(PLATFORM_PATH ${CMAKE_CURRENT_SOURCE_DIR}/platform)

# Include paths
set(INCLUDES 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${FREERTOS_PATH}/include
    ${FREERTOS_PATH}/portable/GCC/ARM_CM0
    ${PLATFORM_PATH}/include
)

# Sources
set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.c
    ${FREERTOS_PATH}/croutine.c
    ${FREERTOS_PATH}/event_groups.c
    ${FREERTOS_PATH}/list.c
    ${FREERTOS_PATH}/queue.c
    ${FREERTOS_PATH}/tasks.c
    ${FREERTOS_PATH}/timers.c
    ${PLATFORM_PATH}/port.c
    ${PLATFORM_PATH}/startup_samd20.c
    ${PLATFORM_PATH}/pal.c
)

# Compilation options
set(COMPILE_OPTIONS
    -mthumb
    -ffunction-sections
    -mlong-calls
    -mcpu=cortex-m0plus
    $<$<CONFIG:RELEASE>:-O2>
    $<$<CONFIG:RELEASE>:-DNDEBUG>
    $<$<CONFIG:DEBUG>:-O0>
    $<$<CONFIG:DEBUG>:-g>
)

# Link options
set(LINK_OPTIONS
    -mthumb
    -Wl,--gc-sections
    -nostartfiles
    -Xlinker -Map=output.map 
)

# Project setup
add_compile_options(${COMPILE_OPTIONS})
add_link_options(${LINK_OPTIONS})
include_directories(${INCLUDES})
add_executable(${PROJECT_NAME} ${SOURCES})
target_linker_script(${PROJECT_NAME} ${PLATFORM_PATH}/samd20g16b_flash.ld)
set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".elf")

# Print sizes
target_print_size(${PROJECT_NAME})
