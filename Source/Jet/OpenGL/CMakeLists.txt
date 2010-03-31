file(GLOB files "*.cpp")
add_library(Jet.OpenGL SHARED ${files})

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
add_definitions(-DFREEGLUT_STATIC)
target_link_libraries(Jet.OpenGL Jet freeglut_d opengl32 glu32 glew32)
else()
add_definitions(-DFREEGLUT_STATIC)
target_link_libraries(Jet.OpenGL Jet freeglut opengl32 glu32 glew32)
endif()

