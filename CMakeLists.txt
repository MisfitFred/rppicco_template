cmake_minimum_required(VERSION 3.20)

option(TARGET_BUILD "Enable unit test" ON)
option(DOC_GENERATION "Enable documentation generation" ON)
option(UNIT_TEST "Enable unit test" OFF)

if(UNIT_TEST AND TARGET_BUILD)
    message(FATAL_ERROR "UNIT_TEST and TARGET_BUILD cannot be enabled at the same time")
endif()

if(TARGET_BUILD)
    message("initialize the Raspberry Pi Pico SDK")

    # note: this must happen before project()
    include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)
else()
    function(pico_add_extra_outputs TARGET) # stubs for unit test
    endfunction()
endif()

project(pico-project-template)

enable_language(C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

if(TARGET_BUILD)
    # initialize the Raspberry Pi Pico SDK
    pico_sdk_init()
endif()

if(DOC_GENERATION)
    # ############ doxygen/sphinx integration: helper function  ##################
    set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doc/doxygen)
    set(DOXYGEN_INDEX_FILE ${DOXYGEN_OUTPUT_DIR}/html/index.html)
    set(DOXYFILE_IN ${CMAKE_CURRENT_SOURCE_DIR}/doc/Doxyfile.in)
    set(DOXYFILE_OUT ${CMAKE_CURRENT_BINARY_DIR}/doc/Doxyfile)

    find_package(Doxygen REQUIRED)
    SET(DOXYGEN_HEADER_FILES CACHE INTERNAL "doxygen header file list")

    function(add_libraries_to_doxygen)
        set(options)
        set(oneValueArgs TARGET)
        set(multiValueArgs)
        cmake_parse_arguments(ADD_LIBRARIES_TO_DOXYGEN "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

        message("**********ADD_LIBRARIES_TO_DOXYGEN_TARGET ${ADD_LIBRARIES_TO_DOXYGEN_TARGET}")
        get_target_property(PUBLIC_HEADER_DIR ${ADD_LIBRARIES_TO_DOXYGEN_TARGET} INTERFACE_INCLUDE_DIRECTORIES)
        message("**********PUBLIC_HEADER_DIR ${PUBLIC_HEADER_DIR}")
        file(GLOB_RECURSE PUBLIC_HEADER ${PUBLIC_HEADER_DIR}/*.h)

        SET(DOXYGEN_HEADER_FILES ${DOXYGEN_HEADER_FILES} ${PUBLIC_HEADER} CACHE INTERNAL "doxygen header file list")
    endfunction()

    # only needed for sphinx yet
    set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
    set(DOXYGEN_GENERATE_XML YES)

    add_subdirectory(doc)
endif()

if(UNIT_TEST)
    # ###################### Integrate gtest and coverage ###########################
    message("************ Unit test enabled ************")

    add_compile_definitions(UNIT_TEST=1)    
    find_package(GTest REQUIRED PATHS /usr/lib /usr/include/gmock /usr/include/gtest /usr/lib/cmake/GTest)
    include(CheckIncludeFileCXX)
    include(GoogleTest)
    include(CTest)
    enable_testing()

    function(add_gtest)
        set(options DISABLE)
        set(oneValueArgs TARGET)
        set(multiValueArgs LIBRARY FILES)
        cmake_parse_arguments(ADD_GTEST "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

        if(NOT ADD_GTEST_DISABLE)
            add_executable(${ADD_GTEST_TARGET} ${ADD_GTEST_FILES})
            target_link_libraries(${ADD_GTEST_TARGET} ${ADD_GTEST_LIBRARY})

            set_target_properties(${ADD_GTEST_TARGET} PROPERTIES LINK_FLAGS "--coverage")
            set_target_properties(${ADD_GTEST_TARGET} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
            target_link_libraries(${ADD_GTEST_TARGET} GTest::gtest GTest::gtest_main Threads::Threads)
            gtest_discover_tests(${ADD_GTEST_TARGET})
        else()
            message(WARNING, "UNIT TEST \"${ADD_GTEST_TARGET}\" DISABLED")
        endif()
    endfunction()

    add_custom_target(cov
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/coverage
        COMMAND gcovr -r ../ --html-details ./coverage/cov.html --html-details-syntax-highlightin -e '.*_test.cpp' .
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
else()
    function(add_gtest)
    endfunction()
endif()

add_subdirectory(components)

if(DOC_GENERATION)
    # ############ doxygen integration: main target ##################
    message("################## ${DOXYGEN_HEADER_FILES}")
    find_package(Doxygen)

    if(DOXYGEN_FOUND)
        set(DOXYGEN_GENERATE_HTML YES)
        set(DOXYGEN_GENERATE_MAN NO)

        doxygen_add_docs(
            Doxygen
            ${DOXYGEN_HEADER_FILES}
            COMMENT "Generate man pages"
        )
    else(DOXYGEN_FOUND)
        message("Doxygen need to be installed to generate the doxygen documentation")
    endif(DOXYGEN_FOUND)
endif()