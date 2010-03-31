file(GLOB files "*.cpp")


add_subdirectory("Test")

add_definitions(/DJETDLL)
add_subdirectory("Core")
#add_subdirectory("Lua")
add_subdirectory("OpenGL")

include_directories("D:\\Include\\Lua")
add_library(Jet SHARED ${files})
#add_executable(Jet ${files})
