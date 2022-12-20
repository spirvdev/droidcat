add_library(casper3 SHARED)
target_sources(casper3 PRIVATE c3_interface.c)

droidcat_add_module(casper3 10 ${CMAKE_PROJECT_VERSION})
