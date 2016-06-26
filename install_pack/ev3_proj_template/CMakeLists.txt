project(%{ProjectName})
cmake_minimum_required(VERSION 2.8)

aux_source_directory(. SRC_LIST)

set(EV3C_DATA_DIRECTORY /usr/local/ev3_arm)
set(CROSS_DIRECTORY /usr/arm-linux-gnueabi)

include_directories(${CROSS_DIRECTORY}/include)
include_directories(${EV3C_DATA_DIRECTORY}/include)

link_directories(${CROSS_DIRECTORY}/lib)
link_directories(${EV3C_DATA_DIRECTORY}/lib)

add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} ev3c)
