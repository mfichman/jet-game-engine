-- This file declares engine options.  Only two functions should be called from
-- this file: 


engine:option("vsync", false)
engine:option("shadow_texture_size", 1024)
engine:option("shadow_distance", 25)
engine:option("shadows_enabled", true)
engine:option("display_width", 1280)
engine:option("display_height", 800)
engine:option("fullscreen", true)
engine:option("shaders_enabled", true)
--[[
engine:option("display_width", 1680)
engine:option("display_height", 1050)
engine:option("fullscreen", true)
]]
engine:option("window_title", "Extreme Asteroids")


engine:search_folder("../Textures")
engine:search_folder("../Meshes")
engine:search_folder("../Scripts")
engine:search_folder("../Sounds")
engine:search_folder("../Shaders")
engine:search_folder("../Music")
engine:search_folder("../Blender")
