-- This file declares engine options.  Only two functions should be called from
-- this file: 


engine:option("vsync_enabled", false)
engine:option("shadow_texture_size", 4096)
engine:option("shadow_distance", 600)
engine:option("shadows_enabled", true)
engine:option("display_width", 1280)
engine:option("display_height", 800)
engine:option("fullscreen_enabled", false)
engine:option("shaders_enabled", true)
engine:option("normal_mapping_enabled", true)
engine:option("specular_mapping_enabled", true)
engine:option("fsaa_enabled", false)
engine:option("fsaa_samples", 4)
engine:option("window_title", "Extreme Asteroids")
engine:option("gravity", 0)


--[[
engine:option("display_width", 1680)
engine:option("display_height", 1050)
engine:option("fullscreen", true)
]]

engine:search_folder("../Textures")
engine:search_folder("../Meshes")
engine:search_folder("../Scripts")
engine:search_folder("../Sounds")
engine:search_folder("../Shaders")
engine:search_folder("../Music")
engine:search_folder("../Blender")
