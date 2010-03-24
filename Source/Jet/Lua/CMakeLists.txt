file(GLOB files "*.cpp")
include_directories("D:\\Include\\Lua")

add_library(Plugin.Lua SHARED ${files})
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
target_link_libraries(Plugin.Lua Jet lua_d luabind_d)
else()
target_link_libraries(Plugin.Lua Jet lua luabind)
endif()
