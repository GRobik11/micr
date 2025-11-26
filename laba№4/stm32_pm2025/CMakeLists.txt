cmake_minimum_required(VERSION 3.12)

set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)

set(ARM_TOOLCHAIN_DIR $ENV{ARM_TOOLCHAIN_PATH})
set(CMAKE_C_COMPILER ${ARM_TOOLCHAIN_DIR}arm-none-eabi-gcc)
set(OBJCOPY_TOOL ${ARM_TOOLCHAIN_DIR}arm-none-eabi-objcopy)
set(ST_FLASH st-flash)

project(stm32_project)

set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS)
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS)

set(OUTPUT_NAME ${PROJECT_NAME})

set(LIB_OUT_DIR ${PROJECT_BINARY_DIR}/lib)
set(EXE_OUT_DIR ${PROJECT_BINARY_DIR}/build)

set(CMAKE_EXECUTABLE_SUFFIX ".elf")

set(CMAKE_C_FLAGS "-mcpu=cortex-m3 -mthumb -std=c99")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fdata-sections -ffunction-sections")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mlittle-endian -g3 -fno-move-loop-invariants")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsigned-char -ffreestanding")

set(LD_FLAGS "-nostdlib -nostartfiles")
set(LD_FLAGS "${LD_FLAGS} -Xlinker -Map -Xlinker ${OUTPUT_NAME}.map")
set(LD_FLAGS "${LD_FLAGS} -Xlinker -T ${CMAKE_CURRENT_SOURCE_DIR}/stm32f103c8.ld")
set(LD_FLAGS "${LD_FLAGS} -Wl,--gc-sections")

set(CMAKE_C_LINK_FLAGS ${LD_FLAGS})

message("BUILD DIR ${EXE_OUT_DIR}")

add_definitions(-DSTM32F103C8)
add_definitions(-DSTM32F10X_MD)

include_directories(cmsis)
include_directories(stm32)

set(SOURCE_FILES
    stm32/startup/startup_stm32f10x_md.c
    stm32/system_stm32f10x.c
    syscalls/syscalls.c
    src/main.c
    src/ssd1306.c
    src/spi.c
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/.gdbinit DESTINATION ${EXE_OUT_DIR})

add_executable(${OUTPUT_NAME} ${SOURCE_FILES})

set(BIN_FILE "${EXE_OUT_DIR}/${OUTPUT_NAME}.bin")
set(HEX_FILE "${EXE_OUT_DIR}/${OUTPUT_NAME}.hex")
set(ELF_FILE "${EXE_OUT_DIR}/${OUTPUT_NAME}${CMAKE_EXECUTABLE_SUFFIX}")

add_custom_command(OUTPUT ${BIN_FILE}
                  COMMAND ${OBJCOPY_TOOL} -O binary ${ELF_FILE} ${BIN_FILE}
                  DEPENDS ${ELF_FILE} ${OUTPUT_NAME})

add_custom_target(bin DEPENDS ${BIN_FILE})

add_custom_command(OUTPUT ${HEX_FILE}
                  COMMAND ${OBJCOPY_TOOL} -O ihex ${ELF_FILE} ${HEX_FILE}
                  DEPENDS ${ELF_FILE} ${OUTPUT_NAME})

add_custom_target(hex DEPENDS ${HEX_FILE})

add_custom_command(OUTPUT flash_cmd
        COMMAND ${ST_FLASH} write ${BIN_FILE} 0x8000000
        DEPENDS ${BIN_FILE})

add_custom_target(flash DEPENDS flash_cmd)