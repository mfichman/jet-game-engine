file(GLOB files "*.cpp")
add_library(FMOD SHARED ${files})
target_link_libraries(FMOD Jet fmodex)
