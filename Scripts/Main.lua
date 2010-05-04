require 'Test'


--ocal mesh = engine:mesh("plane")

--[[
local v = Vertex()
v.normal = Vector(0, 0, 1);
v.tangent = Vector(0, 1, 0);
v.position = Vector(10, -10, -2);
v.texcoord = Texcoord(1, 0);
mesh:vertex(0, v);

v.normal = Vector(0, 0, 1);
v.tangent = Vector(0, 1, 0);
v.position = Vector(10, 10, -2);
v.texcoord = Texcoord(1, 1);..\binary\
mesh:vertex(1, v);


v.normal = Vector(0, 0, 1);
v.tangent = Vector(0, 1, 0);
v.position = Vector(-10, 10, -2);
v.texcoord = Texcoord(0, 1);
mesh:vertex(2, v);


v.normal = Vector(0, 0, 1);
v.tangent = Vector(0, 1, 0);
v.position = Vector(-10, -10, -2);
v.texcoord = Texcoord(0, 0);
mesh:vertex(3, v);

mesh:index(0, 0);
mesh:index(1, 2);
mesh:index(2, 3);

mesh:index(3, 0);
mesh:index(4, 1);
mesh:index(5, 2);
]]

--engine.simulation_speed = 1.0/4.0


Test()