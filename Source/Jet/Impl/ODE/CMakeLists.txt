file(GLOB files "*.cpp")
add_library(Physics_ODE SHARED ${files})
target_link_libraries(Physics_ODE Jet ode)
