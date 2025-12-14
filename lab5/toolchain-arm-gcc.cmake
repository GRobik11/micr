# toolchain-arm-gcc.cmake

# Cross-compiling for ARM Cortex-M
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# Укажи путь к тулчейну ARM (исправь путь, если твой другой)
set(TOOLCHAIN_PREFIX "C:/msys64/mingw64/bin/arm-none-eabi")

set(CMAKE_C_COMPILER "${TOOLCHAIN_PREFIX}-gcc.exe")
set(CMAKE_CXX_COMPILER "${TOOLCHAIN_PREFIX}-g++.exe")
set(CMAKE_ASM_COMPILER "${TOOLCHAIN_PREFIX}-gcc.exe")

set(CMAKE_FIND_ROOT_PATH "${TOOLCHAIN_PREFIX}")

# Отключаем поиск системных include и библиотек хоста
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# Убираем Windows-специфичные флаги линковки
set(CMAKE_EXE_LINKER_FLAGS "")
