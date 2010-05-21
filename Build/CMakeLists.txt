cmake_minimum_required(VERSION 2.6)
project(Jet)
set(CMAKE_BUILD_TYPE Debug)
#set(CMAKE_BUILD_TYPE Release)
set(EXECUTABLE_OUTPUT_PATH ${Jet_SOURCE_DIR}/../Binary)
set(LIBRARY_OUTPUT_PATH ${Jet_SOURCE_DIR}/../Binary)

if(WIN32 AND NOT UNIX)
include_directories(C:\\Include)
include_directories(D:\\Include)
include_directories(C:\\Include\\lua)
include_directories(D:\\Include\\Lua)
include_directories(C:\\Include\\Bullet)
include_directories(D:\\Include\\Bullet)
set(LIB_DIRS ${LIB_DIRS} C:\\Libraries)
set(LIB_DIRS ${LIB_DIRS} D:\\Libraries)
set(LIB_DIRS ${LIB_DIRS} C:\\Libraries\\Debug)
set(LIB_DIRS ${LIB_DIRS} D:\\Libraries\\Debug)
add_definitions(/DWINDOWS)
add_definitions(/D_SCL_SECURE_NO_WARNINGS)
add_definitions(/D_DEBUG)
add_definitions(/D_CRTDBG_MAP_ALLOC)
endif()

if(UNIX AND APPLE)
add_definitions(-DDARWIN)
add_definitions(-g)
add_definitions(-Wall -Werror)
include_directories(/usr/local/include/Bullet/)
include_directories(/Developer/FMOD Programmers API Mac/api/inc) 
endif()

if(UNIX AND NOT APPLE)
add_definitions(-DLINUX)
add_definitions(-g)
add_definitions(-Wall -Werror)
endif()

link_directories(${LIB_DIRS})
include_directories(${Jet_SOURCE_DIR}/../Include)
add_definitions(-DFREEGLUT_STATIC)


file(GLOB files
    "../Source/Jet/*.cpp"
    "../Source/Jet/Types/*.cpp"
    "../Source/Jet/Physics/*.cpp"
    "../Source/Jet/Audio/*.cpp"
    "../Source/Jet/Script/*.cpp"
    "../Source/Jet/Graphics/*.cpp"
    "../Source/Jet/Input/*.cpp"
    "../Source/Jet/Network/*.cpp"
    "../Source/Jet/Core/*.cpp")
add_library(Jet ${files})

file(GLOB files "../Source/Jet/Test/*.cpp")
add_executable(Test ${files})


find_library(GL NAMES OpenGL opengl32 PATHS ${LIB_DIRS})
find_library(GLU NAMES glu32 GLU PATHS ${LIB_DIRS})
find_library(GLEW NAMES glew32 GLEW PATHS ${LIB_DIRS})
find_library(SDL NAMES SDL PATHS ${LIB_DIRS})
find_library(SDL_MAIN NAMES SDLmain PATHS ${LIB_DIRS})
find_library(SDL_IMAGE NAMES SDL_image PATHS ${LIB_DIRS})
find_library(BOOST_SYSTEM NAMES boost_system PATHS ${LIB_DIRS})
find_library(BOOST_FILESYSTEM NAMES boost_filesystem PATHS ${LIB_DIRS})
find_library(FMOD_EX NAMES fmod FMOD fmodex PATHS ${LIB_DIRS})
find_library(FREETYPE NAMES freetype PATHS ${LIB_DIRS})

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
find_library(BULLET_COLLISION NAMES libbulletcollision_d bulletcollision PATHS ${LIB_DIRS})
find_library(BULLET_DYNAMICS NAMES libbulletdynamics_d bulletdynamics PATHS ${LIB_DIRS})
find_library(BULLET_MATH NAMES libbulletmath_d bulletmath PATHS ${LIB_DIRS})
find_library(LUA NAMES lua_d lua PATHS ${LIB_DIRS})
find_library(LUABIND NAMES luabind_d luabind PATHS ${LIB_DIRS})
else()
find_library(BULLET_COLLISION NAMES libbulletcollision bulletcollision PATHS ${LIB_DIRS})
find_library(BULLET_DYNAMICS NAMES libbulletdynamics bulletdynamics PATHS ${LIB_DIRS})
find_library(BULLET_MATH NAMES libbulletmath bulletmath PATHS ${LIB_DIRS})
find_library(LUA NAMES lua PATHS ${LIB_DIRS})
find_library(LUABIND NAMES luabind PATHS ${LIB_DIRS})
endif()


set(LIBRARIES ${LIBRARIES} ${GL})
set(LIBRARIES ${LIBRARIES} ${GLU})
set(LIBRARIES ${LIBRARIES} ${GLEW})
set(LIBRARIES ${LIBRARIES} ${SDL})
set(LIBRARIES ${LIBRARIES} ${SDL_MAIN})
set(LIBRARIES ${LIBRARIES} ${SDL_IMAGE})
set(LIBRARIES ${LIBRARIES} ${BULLET_COLLISION})
set(LIBRARIES ${LIBRARIES} ${BULLET_DYNAMICS})
set(LIBRARIES ${LIBRARIES} ${BULLET_MATH})
set(LIBRARIES ${LIBRARIES} ${LUA})
set(LIBRARIES ${LIBRARIES} ${LUABIND})
set(LIBRARIES ${LIBRARIES} ${FMOD_EX})
set(LIBRARIES ${LIBRARIES} ${FREETYPE})

if(APPLE)
find_library(COCOA NAMES Cocoa PATHS ${LIB_DIRS})
set(LIBRARIES ${LIBRARIES} ${BOOST_SYSTEM})
set(LIBRARIES ${LIBRARIES} ${BOOST_FILESYSTEM})
set(LIBRARIES ${LIBRARIES} ${COCOA})
endif()

if(WIN32)
find_library(WINSOCK NAMES ws2_32 PATHS ${LIB_DIRS})
set(LIBRARIES ${LIBRARIES} ${WINSOCK})
endif()

target_link_libraries(Jet ${LIBRARIES})
target_link_libraries(Test Jet)
