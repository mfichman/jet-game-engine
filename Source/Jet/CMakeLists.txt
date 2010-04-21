file(GLOB files "*.cpp" "Core/*.cpp")

add_library(Jet ${files})

add_definitions(-DFREEGLUT_STATIC)

find_library(GL NAMES OpenGL opengl32)
find_library(GLU NAMES glu32 GLU)
find_library(GLEW NAMES glew32 GLEW)
find_library(IL NAMES IL DevIL PATHS ${CMAKE_LINK_DIRECTORIES})
find_library(SDL2 NAMES SDLmain)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
find_library(LUA NAMES lua_d lua)
find_library(LUABIND NAMES luabind_d luabindd)
find_library(BULLET_COLLISION NAMES libbulletcollision_d bulletcollision)
find_library(BULLET_DYNAMICS NAMES libbulletdynamics_d bulletdynamics)
find_library(BULLET_MATH NAMES libbulletmath_d bulletmath)
else()
find_library(LUA NAMES lua)
find_library(LUABIND NAMES luabind)
find_library(BULLET_COLLISION NAMES libbulletcollision bulletcollision)
find_library(BULLET_DYNAMICS NAMES libbulletdynamics bulletdynamics)
find_library(BULLET_MATH NAMES libbulletmath bulletmath)
endif()

add_subdirectory("Test")

target_link_libraries(Jet ${IL} ${LUA} ${LUABIND} ${BULLET_COLLISION}
    ${BULLET_DYNAMICS} ${BULLET_MATH} ${GL} ${GLU} ${GLEW} SDL)

