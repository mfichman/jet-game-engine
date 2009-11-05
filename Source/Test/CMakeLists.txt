file(GLOB files "*.cpp")

add_executable(Test ${files})
target_link_libraries(Test Jet)
