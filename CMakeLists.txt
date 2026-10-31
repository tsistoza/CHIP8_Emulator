cmake_minimum_required(VERSION 3.23)

project(CHIP8Emulator VERSION 1.0)

option(TESTING_MODE "This will compile the tests suite" OFF)

# LIBRARIES
add_library(CHIP8)
target_sources(CHIP8
    PRIVATE
        chip8.cpp
    PUBLIC
        FILE_SET HEADERS
        FILES
            chip8.hpp
)

find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)
add_library(PLATFORM)
target_sources(PLATFORM
    PRIVATE
        platform.cpp
    PUBLIC
        FILE_SET HEADERS
        FILES
            platform.hpp
)
target_link_libraries(PLATFORM 
    PRIVATE
        CHIP8
        SDL3::SDL3
)

# TEST MAIN
if (TESTING_MODE)
    message("IN TESTING MODE,  BUILDING TEST MAIN")
    add_executable(TestMain)
    
    target_sources(TestMain
        PRIVATE
            test.cpp

        PUBLIC
            FILE_SET HEADERS
            FILES
                test.hpp
    )

    target_link_libraries(TestMain
        PRIVATE
            CHIP8
    )
endif()

if (NOT TESTING_MODE)
    add_executable(Main)
    target_sources(Main
        PRIVATE
            main.cpp
    )
    target_link_libraries(Main
        PRIVATE
            CHIP8
            PLATFORM
    )

    # INSTALL EXECUTABLE
    install (
        TARGETS Main
        DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    )

    # Create DEPENDENCY LIST
    install(
        TARGETS PLATFORM CHIP8
        EXPORT MainTargets
        FILE_SET HEADERS
    )

    # INSTALL Dependency LIST INTO INSTALL FOLDER
    install(
        EXPORT MainTargets
        DESTINATION ${CMAKE_INSTALL_PREFIX}
        NAMESPACE Chip8::
    )

    # CREATE PACKAGER VERSION FOR COMPATIBILITY
    include (CMakePackageConfigHelpers)
    write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/Chip8ConfigVersion.cmake
        COMPATIBILITY ExactVersion
    )

    install (
        FILES
            ${CMAKE_CURRENT_BINARY_DIR}/Chip8ConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_PREFIX}
    )
endif()