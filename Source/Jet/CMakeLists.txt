file(GLOB files "*.cpp" "Core/*.cpp")


include_directories("D:\\Include\\Lua")
include_directories("D:\\Include\\Bullet")

add_library(Jet ${files})
add_definitions(-DFREEGLUT_STATIC)

find_library(GL NAMES OpenGL opengl32)
find_library(GLU NAMES glu32 GLU)
find_library(GLEW NAMES glew32 GLEW)
find_library(IL NAMES IL DevIL)
find_library(BOOST_FILESYSTEM NAMES boost_filesystem)
find_library(BOOST_SYSTEM NAMES boost_system)
find_library(FREEGLUT NAMES freeglut)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
find_library(LUA NAMES lua_d lua)
find_library(LUABIND NAMES luabind_d luabindd)
find_library(BULLETCOLLISION NAMES libbulletcollision_d bulletcollision)
find_library(BULLETDYNAMICS NAMES libbulletdynamics_d bulletdynamics)
find_library(BULLETMATH NAMES libbulletmath_d bulletmath)
else()
find_library(LUA NAMES lua)
find_library(LUABIND NAMES luabind)
find_library(BULLETCOLLISION NAMES libbulletcollision bulletcollision)
find_library(BULLETDYNAMICS NAMES libbulletdynamics bulletdynamics)
find_library(BULLETMATH NAMES libbulletmath bulletmath)
endif()


target_link_libraries(Jet 
	${IL} ${LUA} ${LUABIND} ${X11} 
	${BULLETCOLLISION} ${BULLETDYNAMICS} ${BULLETMATH} 
	${GL} ${GLU} ${FREEGLUT} ${GLEW}
	${BOOST_FILESYSTEM} ${BOOST_SYSTEM})

add_subdirectory("Test")
