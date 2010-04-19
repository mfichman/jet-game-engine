require 'Test'

local material = engine:material("plane")
material.diffuse_color = Color(1, 1, 1, 1)
material.diffuse_map = "MetalDiffuse.png"
material.normal_map = "MetalNormal.png"

local mesh = engine:mesh("plane")
local v = Vertex()
v.normal = Vector(1, 0, 0);
v.tangent = Vector(0, 0, 1);
v.position = Vector(-3, 10, -10);
v.texcoord = Texcoord(1, 0);
mesh:vertex(0, v);

v.normal = Vector(1, 0, 0);
v.tangent = Vector(0, 1, 0);
v.position = Vector(-3, 10, 10);
v.texcoord = Texcoord(1, 1);
mesh:vertex(1, v);

v.normal = Vector(1, 0, 0);
v.tangent = Vector(0, 0, 1);
v.position = Vector(-3, -10, 10);
v.texcoord = Texcoord(0, 1);
mesh:vertex(2, v);

v.normal = Vector(1, 0, 0);
v.tangent = Vector(0, 1, 0);
v.position = Vector(-3, -10, -10);
v.texcoord = Texcoord(0, 0);
mesh:vertex(3, v);

mesh:index(0, 0);
mesh:index(1, 2);
mesh:index(2, 3);

mesh:index(3, 0);
mesh:index(4, 1);
mesh:index(5, 2);


Test()