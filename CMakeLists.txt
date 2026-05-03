cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)


# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME STM32G031F6P6-eink-2.13)

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Enable CMake support for ASM and C languages
enable_language(C ASM)

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME}
)

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
)

# Add sources to executable
file(GLOB_RECURSE EINK_SOURCES "Components/eink/*.c")
file(GLOB_RECURSE EINK_GFX_SOURCES "Components/eink_gfx/*.cpp")
file(GLOB_RECURSE Adafruit_GFX_SOURCES "Components/Adafruit_GFX/*.cpp")
list(FILTER Adafruit_GFX_SOURCES EXCLUDE REGEX "examples/.*")
list(FILTER Adafruit_GFX_SOURCES EXCLUDE REGEX "fontconvert/.*")
list(FILTER Adafruit_GFX_SOURCES EXCLUDE REGEX "Adafruit_SPITFT\\.cpp")
list(FILTER Adafruit_GFX_SOURCES EXCLUDE REGEX "Adafruit_GrayOLED\\.cpp")
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    ${EINK_SOURCES}
    ${EINK_GFX_SOURCES}
    ${Adafruit_GFX_SOURCES}
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Components/eink/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/Components/eink/port
    ${CMAKE_CURRENT_SOURCE_DIR}/Components/eink_gfx
    ${CMAKE_CURRENT_SOURCE_DIR}/Components/Adafruit_GFX
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined symbols
)

# Remove wrong libob.a library dependency when using cpp files
list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx

    # Add user defined libraries
)
