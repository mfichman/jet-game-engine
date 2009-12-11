file(GLOB files "*.cpp")
add_library(Ode SHARED ${files})
target_link_libraries(Ode Jet ode)
