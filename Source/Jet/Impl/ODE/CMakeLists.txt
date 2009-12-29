file(GLOB files "*.cpp")
add_library(ODE SHARED ${files})
target_link_libraries(ODE Jet ode)
