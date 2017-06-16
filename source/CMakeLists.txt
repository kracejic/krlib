
# Flags
if(NOT MSVC)
    add_compile_options(-Wall -Wextra -Wshadow -Wnon-virtual-dtor -Wunused -pedantic)
    #-Wmisleading-indentation 
endif()

add_subdirectory(unittest)


#------------------------------------------------------------------------------
# Other MISC targets
#------------------------------------------------------------------------------

file(GLOB_RECURSE ALL_SOURCE_FILES *.cpp *.cc *.c)
file(GLOB_RECURSE ALL_HEADER_FILES *.h *.hpp)

# Static analysis via clang-tidy target
# We check for program, since when it is not here, target makes no sense
find_program(TIDY_PATH clang-tidy PATHS /usr/local/Cellar/llvm/*/bin)
if(TIDY_PATH)
    message(STATUS "clang-tidy - static analysis              YES ")
    add_custom_target(tidy
        COMMAND ${TIDY_PATH} ${ALL_SOURCE_FILES} -p=./ )
else()
    message(STATUS "clang-tidy - static analysis              NO ")
endif()

# cpp check static analysis
find_program(CPPCHECK_PATH cppcheck)
if(CPPCHECK_PATH)
    message(STATUS "cppcheck - static analysis                YES ")
    add_custom_target(
        cppcheck
        COMMAND ${CPPCHECK_PATH}
        --enable=warning,performance,portability,information,missingInclude
        --std=c++11
        --template=gcc
        --verbose
        --quiet
        ${ALL_SOURCE_FILES} ${ALL_HEADER_FILES}
        )
else()
    message(STATUS "cppcheck - static analysis                NO ")
endif()

# run clang-format on all files
find_program(FORMAT_PATH clang-format)
if(FORMAT_PATH)
    message(STATUS "clang-format - code formating             YES ")
    add_custom_target(format
        COMMAND ${FORMAT_PATH} -i ${ALL_SOURCE_FILES} ${ALL_HEADER_FILES} )
else()
    message(STATUS "clang-format - code formating             NO ")
endif()


