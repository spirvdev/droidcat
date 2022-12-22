
function(droidcat_add_module MODULE_OBJECT MODULE_ID MODULE_VERSION)
    set_target_properties(${MODULE_OBJECT} PROPERTIES PREFIX ${MODULE_ID}-)
    set_target_properties(${MODULE_OBJECT} PROPERTIES SUFFIX -${MODULE_VERSION}.dm)
    target_include_directories(${MODULE_OBJECT} PRIVATE ${CMAKE_SOURCE_DIR}/include)

endfunction()
