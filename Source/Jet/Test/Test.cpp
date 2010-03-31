#include <Jet/Engine.hpp>
#include <iostream>
#include <stdexcept>

void main() {
    try {
        Jet::EnginePtr engine(new Jet::Engine);
        engine->module("Jet.Core");
        engine->module("Jet.OpenGL");
    } catch (std::exception& ex) {
        std::cout << ex.what() << std::endl;
    }
}
