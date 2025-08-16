set(TEST_NAME ${COMPONENT}.test)

file(GLOB SOURCES "*.h" "*.cpp")

add_executable(${TEST_NAME} ${SOURCES})
target_link_libraries(${TEST_NAME} PRIVATE ${COMPONENT} GTest::gtest_main)
target_include_directories(${TEST_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})