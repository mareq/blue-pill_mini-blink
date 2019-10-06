set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

#set(CMAKE_SYSROOT /path/to/target/system/root/)
#set(CMAKE_STAGING_PREFIX /path/to/staging/area/on/host/)

set(TARGET_TRIPLE "arm-none-eabi")
set(CMAKE_ASM_COMPILER ${TARGET_TRIPLE}-gcc)
set(CMAKE_C_COMPILER ${TARGET_TRIPLE}-gcc)
set(CMAKE_CXX_COMPILER ${TARGET_TRIPLE}-g++)
#LD := ${TARGET_TRIPLE}-gcc
#AR := ${TARGET_TRIPLE}-ar
#AS := ${TARGET_TRIPLE}-as

set(CMAKE_OBJCOPY ${TARGET_TRIPLE}-objcopy)
set(CMAKE_OBJDUMP ${TARGET_TRIPLE}-objdump)
set(CMAKE_OBJSIZE ${TARGET_TRIPLE}-size)
set(CMAKE_DEBUGGER ${TARGET_TRIPLE}-gdb)
set(CMAKE_CPPFILT ${TARGET_TRIPLE}-c++filt)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)


# vim: set ts=2 sw=2 et:


