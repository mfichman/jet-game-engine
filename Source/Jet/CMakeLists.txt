file(GLOB files "*.cpp" "Core/*.cpp")


include_directories("D:\\Include\\Lua")
include_directories("D:\\Include\\Bullet")

add_library(Jet ${files})

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
add_definitions(-DFREEGLUT_STATIC)
target_link_libraries(
    Jet DevIL freeglut_d opengl32 glu32 glew32 lua_d luabind_d
    libbulletcollision_d libbulletmath_d libbulletdynamics_d)

else()
add_definitions(-DFREEGLUT_STATIC)
target_link_libraries(Jet DevIL freeglut opengl32 glu32 glew32 lua luabind
    libbulletcollision, libbulletmath libbulletdynamics)

endif()



add_subdirectory("Test")
