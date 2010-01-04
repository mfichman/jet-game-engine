file(GLOB files "*.cpp")

if(WIN32 AND NOT UNIX)
add_definitions(/DJETDLL)
endif()

add_library(Jet SHARED ${files})



#target_link_libraries(Jet dl)


add_subdirectory(Impl)
