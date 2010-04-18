
class 'Actor'

function Actor:__init(node, name)
    __adopt_actor(self, node, name)
end

function Actor:on_update() end
function Actor:on_render() end
function Actor:on_collision() end
function Actor:on_destroy() end

class 'Starship' (Actor)

function Starship:__init(node, name)
    Actor.__init(self, node, name)
    
    self.node.position = Vector(0, math.random(-20, 20), 0)

    self.hull = self.node:node("mesh")
    self.hull_mesh = self.hull:mesh_object("mesh")
    self.hull_mesh.mesh = "Shark.obj"
    self.hull_mesh.material = "Box.mtl"

    self.pos = 0
    
    self.body = self.node:rigid_body()
    self.body.mass = 10.0
end

function Starship:on_update()
    self.body:apply_force(Vector(0, 1000, 0))
    --[[self.pos = self.pos + 0.01
    local q0 = Quaternion(Vector(0, 1, 0), 3.1*self.pos)
    local q1 = Quaternion(Vector(1, 0, 0), 2.0*self.pos)
    self.hull.rotation = q0*q1
    self.node.rotation = q0]]
end



for i=1,2 do
Starship(engine.root, "test"..i)
end

