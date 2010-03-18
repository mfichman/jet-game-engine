file(GLOB files "*.cpp")

if(WIN32 AND NOT UNIX)
add_definitions(/DJETDLL)
endif()

include_directories("D:\\Include\\Lua")
add_library(Plugin.Lua SHARED ${files})
target_link_libraries(Plugin.Lua Jet lua_d luabind_d)
