-- This file declares engine options.  Only two functions should be called from
-- this file: 


engine:option("shadow_texture_size", 1024)
engine:option("shadows_enabled", true)
engine:option("display_width", 1200)
engine:option("display_height", 800)
engine:option("window_title", "Extreme Asteroids")
engine:option("fullscreen", false)

engine:search_folder("../Textures")
engine:search_folder("../Meshes")
engine:search_folder("../Scripts")
engine:search_folder("../Sounds")
engine:search_folder("../Shaders")
engine:search_folder("../Music")
engine:search_folder("../Blender")