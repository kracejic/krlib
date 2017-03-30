


# Description of the directory structure

* mainProjectFolder
    - **build** - user created, build takes place here
        + **dist** - here builded application is copied (by deafult, see CMAKE_INSTALL_PREFIX in main CMakeLists.txt)
            * **bin**
            * **share**
                - **data** - here folder data is copied
            * **doc**
                - doxygen lives here documentation
    - **doc** - contains documentation
        + CMakeLists.txt - doc building
        + **doxygen**
            * doxygen settings, and modern style config
    - **test** - integration tests, CTest, data sets for tests and unit tests
        + CMakeLists.txt - tests specification
    - **external**
        + CMakeLists.txt - external projects handling (eg. Download of Catch)
    - readme.md - main readme file
    - CMakeLists.md - Main CMake configuration
    - .clang-format - clang format config file
    - .clang-tidy - clang tidy config file
    - .travis.yml - continuous integration configuration


