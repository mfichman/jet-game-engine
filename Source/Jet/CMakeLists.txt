file(GLOB files "*.cpp")

if(WIN32 AND NOT UNIX)
add_definitions(/DJETDLL)
endif()

add_subdirectory("Lua")
add_subdirectory("OpenGL")

include_directories("D:\\Include\\Lua")
add_library(Jet SHARED ${files})
