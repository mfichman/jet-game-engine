file(GLOB files "*.cpp")
add_library(Audio_FMOD SHARED ${files})
target_link_libraries(Audio_FMOD Jet fmodex)
