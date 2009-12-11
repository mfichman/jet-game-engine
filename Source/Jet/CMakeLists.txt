file(GLOB files "*.cpp" "Graphics/*.cpp")

add_library(Jet ${files})

add_subdirectory(Impl)
