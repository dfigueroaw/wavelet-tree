add_library(unity STATIC ${PROJECT_SOURCE_DIR}/external/unity/unity.c)
target_include_directories(unity INTERFACE ${PROJECT_SOURCE_DIR}/external/unity)

set(TEST_SOURCES test_wavelet.c)

foreach(TEST_SOURCE ${TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
    set(TEST_EXEC ${TEST_NAME})

    add_executable(${TEST_EXEC} ${TEST_SOURCE})
    target_link_libraries(${TEST_EXEC} PRIVATE wavelet-tree)
    target_link_libraries(${TEST_EXEC} PRIVATE unity)

    add_test(NAME ${TEST_NAME} COMMAND ${TEST_EXEC})
endforeach()
