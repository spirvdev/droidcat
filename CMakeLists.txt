cmake_minimum_required(VERSION 3.21)

project(droidcat LANGUAGES C VERSION 0.0.1)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_C_STANDARD_REQUIRED ON)

include(cmake/modules.cmake)

add_compile_options(-Wall; -Wno-uninitialized
    "$<$<CONFIG:RELEASE>:-Ofast;>"
    "$<$<CONFIG:DEBUG>:-Og;-ggdb>"
)

add_compile_definitions(
    "$<$<CONFIG:DEBUG>:DROIDCAT_IS_UNDER_DEBUG=1>"
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(droidcat)
add_subdirectory(source)

add_subdirectory(modules/casper3)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/default/ DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/runtime)
