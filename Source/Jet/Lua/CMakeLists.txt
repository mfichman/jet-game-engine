file(GLOB files "*.cpp")
include_directories("D:\\Include\\Lua")

add_library(Module.Lua SHARED ${files})
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
target_link_libraries(Jet.Lua Jet lua_d luabind_d)
else()
target_link_libraries(Jet.Lua Jet lua luabind)
endif()
