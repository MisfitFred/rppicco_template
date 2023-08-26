
if(TARGET_BUILD)
    add_executable(bbq
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    )

    target_link_libraries(bbq PUBLIC pico_stdlib max31865)

    pico_add_extra_outputs(bbq)
endif()

