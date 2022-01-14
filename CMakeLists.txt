cmake_minimum_required(VERSION 3.10)
project("samp-ptl")

OPTION(BUILD_EXAMPLES "Build the examples" OFF)

add_library(${PROJECT_NAME} INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include/samp-ptl/amx/amx.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/samp-ptl/amx/sclinux.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/samp-ptl/plugincommon.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/samp-ptl/ptl.h
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_defenitions(${PROJECT_NAME} PRIVATE HAVE_STDINT_H)

if (BUILD_EXAMPLES)
    add_subdirectory(example)
endif()
