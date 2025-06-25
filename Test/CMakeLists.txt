cmake_minimum_required(VERSION 3.10)
project(embedded_tests C)

set(CMAKE_C_STANDARD 11)

set(UNITY_DIR Unity/Src)

# Add Unity
add_library(Unity ${UNITY_DIR}/unity.c)

# Include dirs
target_include_directories(Unity PUBLIC
    ${UNITY_DIR}
)

set(CIRC_BUF_INC_DIR ../Core/Inc)

# Add your circular buffer source
add_library(circular_buffer INTERFACE)

# Include dirs
target_include_directories(circular_buffer INTERFACE
    ${CIRC_BUF_INC_DIR}
)

# Create test executable
add_executable(test_circular_buffer test_circular_buffer.c)

# Link everything together
target_link_libraries(test_circular_buffer
    circular_buffer
    Unity
)
