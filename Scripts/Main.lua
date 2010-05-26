require 'StartScreen'
require 'Task'
require 'Stats'

-- Set up lights
print("Creating lights")
light_node = engine.root:node();
light = light_node:light("light") {
    ambient_color = Color(.6, .6, .6, 1),
    diffuse_color = Color(1, 1, 1, 1),
    specular_color = Color(1, 1, 1, 1),
    direction = Vector(1, 0, 1)
}

-- Set up the camera
print("Creating camera")
camera_node = engine.root:node("camera")
camera_node.position = Vector(35, 0, 35)
camera_node:look(Vector(-5, -5, 5), Vector(0, 1, 0))
--camera_node:look(Vector(0, 0, 0), Vector(0, 0, 1))
camera = camera_node:camera("camera") {
    active = true,
    field_of_view = 45,
    far_clipping_distance = 1000,
    near_clipping_distance = 0.1
}

-- Set up FPS counter
Stats(engine.screen, "stats")

StartScreen()