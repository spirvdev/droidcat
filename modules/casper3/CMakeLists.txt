add_library(casper3 SHARED)
target_sources(casper3 PRIVATE c3_int.c)

droidcat_add_module(casper3 10 0.0.1)
