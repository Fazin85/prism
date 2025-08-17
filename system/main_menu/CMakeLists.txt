cmake_minimum_required(VERSION 3.15)

# RISC-V cross-compilation
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_C_COMPILER riscv64-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER riscv64-linux-gnu-g++)

project(MainMenu)

set(CMAKE_CXX_FLAGS "-march=rv64imafd -mabi=lp64d -static -std=c++17 -O2")
set(CMAKE_C_FLAGS "-march=rv64imafd -mabi=lp64d -static -O2")

add_executable(main_menu.elf src/main.cpp)

# Show binary size
add_custom_command(TARGET main_menu.elf POST_BUILD
    COMMAND echo "Main Menu compiled successfully:"
    COMMAND ls -lh main_menu.elf
)
