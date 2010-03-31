file(GLOB files "*.cpp")
add_executable(Test ${files})

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
target_link_libraries(Test Jet)
else()
target_link_libraries(Test Jet)
endif()

