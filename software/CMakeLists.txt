# Set minimum required version of CMake
cmake_minimum_required(VERSION 3.12)

# Include build function from Pico SDK
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

# Set name of project in PROJECT_NAME variable
project(tictactoe C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
# Creates a pico-sdk subdir in our proj for libs

if (VERBOSE)
    add_compile_definitions(VERBOSE)
endif()

pico_sdk_init()

add_executable(${PROJECT_NAME}
    src/main.cpp
    ../src/game.cpp
)

# Create map, bin, extra, uf2 files
pico_add_extra_outputs(${PROJECT_NAME})

# Link to pico_stdlib (gpio, time, etc. functions)
target_link_libraries(
    ${PROJECT_NAME}
    pico_stdlib
    pico_multicore
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR} )

# Enable usb output, disable uart output
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

