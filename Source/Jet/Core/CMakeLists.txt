file(GLOB files "*.cpp")
add_library(Jet.Core SHARED ${files})

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
target_link_libraries(Jet.Core Jet DevIL)
else()
target_link_libraries(Jet.Core Jet DevIL)
endif()

