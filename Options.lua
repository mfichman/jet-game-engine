-- This file declares engine options.  Only two functions should be called from
-- this file:

if (true) then
    engine:option("shadow_texture_size", 2048)
    engine:option("display_width", 1680)
    engine:option("display_height", 1050)
    engine:option("fullscreen_enabled", true)
    engine:option("fsaa_enabled", true)
    engine:option("fsaa_samples", 4)
    engine:option("shadows_enabled", true)
    engine:option("normal_mapping_enabled", true)
    engine:option("specular_mapping_enabled", true)
    engine:option("skysphere_texture", "Nebula.png")
else
    engine:option("shadow_texture_size", 1024)
    engine:option("display_width", 800)
    engine:option("display_height", 600)
    engine:option("fullscreen_enabled", false)
    engine:option("fsaa_enabled", false)
    engine:option("fsaa_samples", 0)
    engine:option("shadows_enabled", true)
    engine:option("normal_mapping_enabled", false)
    engine:option("specular_mapping_enabled", false)
    engine:option("skysphere_texture", "")
end

engine:option("vsync_enabled", true)
engine:option("shadow_distance", 400)
engine:option("shadow_cascades", 4)
engine:option("shadow_correction", 0.55)
engine:option("shaders_enabled", true)
engine:option("window_title", "zero combat")
engine:option("gravity", 0)
engine:option("network_smoothness", 0.05)
engine:option("network_packet_rate", 6)
engine:option("input_delay", 6)

--[[
engine:option("display_width", 1680)
engine:option("display_height", 1050)
engine:option("fullscreen", true)
]]

engine:search_folder("Textures")
engine:search_folder("Meshes")
engine:search_folder("Scripts")
engine:search_folder("Scripts/Animation")
engine:search_folder("Scripts/Engine")
engine:search_folder("Scripts/Menus")
engine:search_folder("Scripts/Widgets")
engine:search_folder("Sounds")
engine:search_folder("Shaders")
engine:search_folder("Music")
engine:search_folder("Blender")
engine:search_folder("Fonts")
