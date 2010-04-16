
class 'Starship' (Jet.Controller)

function Starship:__init(node, name)
    Jet.Controller.__init(self, node, name)
    print("Initialized in Lua")
    
    self.mesh_object = self.node:mesh_object(name)
    self.mesh_object.mesh = "Shark.obj"
    self.mesh_object.material = "Box.mtl"
    print(name)
    self.node.position = Jet.Vector(0, math.random(-20, 20), 0)
    self.pos = 0
end

function Starship:on_update()
    --self.pos = self.pos + 0.01
    --print(self)
    --local q0 = Jet.Quaternion(Jet.Vector(0, 1, 0), 3.1*self.pos)
    --local q1 = Jet.Quaternion(Jet.Vector(1, 0, 0), 2.0*self.pos)
    --local q = q0*q1
    --self.node.rotation = q
end


for i=1,100 do
    local s = Starship(Jet.engine.root, "test"..i)
end
