cmake_minimum_required(VERSION 3.10)

project(sampptl_example)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)

include(AddSAMPPlugin)

add_samp_plugin(${PROJECT_NAME} main.cc)
target_link_libraries(${PROJECT_NAME} samp-ptl)