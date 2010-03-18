class 'Starship'

function Starship:__init()
    self.shield_opacity = 0
    self.hit_direction = 0
end

function Starship:on_update()
    self.shield_opacity = math.max(0, self.shield_opacity - delta)
    local shield_color = Color(0.005, 0.01, 0.025, 1)
    if (self.shield_opacity > 0) then
        shield_color = shield_color + 0.5 * self.shield_opacity
        self:get_mesh_object("shield").material.ambient_color = 

        self.shield.material.ambient_color = shield_color
    end
    self.shield.material.diffuse = shield_color

    self:get_particle_system("fire").spawn_enabled = true
    self:get_particle_system("plasma").spawn_enabled = true
    self:get_audio_source("engine").state = Sound.PLAYING
    

    self:create_particle_system("fire")
    self.fire.spawn_enabled = true
    self.plasma.spawn_enabled = true
    self.engine.state = Sound.PLAYING

    self.node.createParticleSystem("fire")

    self.node.
end


function Starship:on_collision()
    
end
