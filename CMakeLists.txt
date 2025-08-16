cmake_minimum_required(VERSION 3.15)
project(Prism VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add subdirectories
add_subdirectory(third_party/libriscv)

# Optional: add tests
if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

# Main executable
add_executable(prism
    src/main.cpp
)

target_include_directories(prism PRIVATE 
    include
    src
)

target_link_libraries(prism
    riscv
)
