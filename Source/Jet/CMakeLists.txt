file(GLOB files "*.cpp")

add_library(Jet SHARED ${files})
target_link_libraries(Jet dl)

add_subdirectory(Impl)
