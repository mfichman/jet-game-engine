



class 'Starship'

function Starship:init()
    self.node = Engine.root:node();
    self.hull = self.node:mesh_object();
    self.shield = self.node:mesh_object();
end


function Starship:on_collision(collision)

end

function Starship: