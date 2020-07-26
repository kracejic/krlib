#------------------------------------------------------------------------------
# Unit tests via Catch framework
#
# For testing on the function/class level.

set(SRCTEST
    testmain.cpp
    base64.cpp
    krstring.cpp
    jsonwritter.cpp
    flatimap.cpp
    explicit.cpp
    optional.cpp
    stopwatch.cpp
    inlineVec.cpp
    inlineRing.cpp
    indexvector.cpp
    jsonReader.cpp
    shared_ptr_st.cpp
    paddedVector.cpp
    fixedString.cpp
    result.cpp
    )

# add_compile_options("-fno-inline")

add_executable(unittests EXCLUDE_FROM_ALL ${SRCTEST})
target_compile_definitions(unittests PRIVATE UNIT_TESTS) #add -DUNIT_TESTS define
# target_compile_options(unittests PRIVATE -fno-access-control)
set_property(TARGET unittests PROPERTY CXX_STANDARD 20) # we want C++17
add_dependencies(unittests Catch) # Catch is needed for unit tests, located in ../external
target_include_directories(unittests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${EXTERNAL_CATCH_INCLUDE_DIR}
    )

# convenience target for running only the unit tests
add_custom_target(unit
    #this way we can use faux data from /test dir (if we hady any):
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/test
    COMMAND $<TARGET_FILE:unittests>
    DEPENDS unittests)
add_custom_target(unitall
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/test
    COMMAND $<TARGET_FILE:unittests> -s -a -r=compact
    DEPENDS unittests)



if(GDB_PATH)
    add_custom_target(unit-gdb
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/test
        USES_TERMINAL
        DEPENDS unittests
        COMMAND ${GDB_PATH} $<TARGET_FILE:unittests>)
endif()
