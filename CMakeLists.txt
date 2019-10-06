# See also: https://github.com/PurpleAlien/stm32-minimal/blob/master/Makefile

cmake_minimum_required(VERSION 3.0)
project (mini-blink)

set(CMAKE_BUILD_TYPE Debug)
message( STATUS "CMAKE_BUILD_TYPE: <${CMAKE_BUILD_TYPE}>" )

set(ELF ${PROJECT_NAME}.elf)

enable_language(ASM)
set(INIT_FILE "init_stm32f103x8.c")

configure_file(${PROJECT_SOURCE_DIR}/stm32f103x8.ld ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.ld)

add_executable(${ELF} ${INIT_FILE} src/main.c)
target_compile_options(${ELF}
  PUBLIC
  -g
  -Wall
  -Werror
  -mthumb
  -mcpu=cortex-m3
  -mfloat-abi=soft
  -mlittle-endian
)
set_target_properties(${ELF}
  PROPERTIES
  LINK_FLAGS " \
     -T${PROJECT_NAME}.ld \
     -mthumb -mcpu=cortex-m3 \
     -Wl,--gc-sections \
     -Wl,-Map=${PROJECT_NAME}.map \
  "
)

add_custom_target(${PROJECT_NAME}.hex ALL DEPENDS ${ELF} COMMAND ${CMAKE_OBJCOPY} -Oihex ${ELF} ${PROJECT_NAME}.hex)
add_custom_target(${PROJECT_NAME}.bin ALL DEPENDS ${ELF} COMMAND ${CMAKE_OBJCOPY} -Obinary ${ELF} ${PROJECT_NAME}.bin)


# vim: set ts=2 sw=2 et:


