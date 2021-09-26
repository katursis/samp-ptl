cmake_minimum_required(VERSION 3.10)

project(sampptl_example)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)

include(AddSAMPPlugin)

add_samp_plugin(${PROJECT_NAME} main.cc)
