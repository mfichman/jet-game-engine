# Jet Game Engine

A hobby game engine. Right now it has support for DirectX 9, FMOD, eXpat XML, Lua and shaders. It comes with a sample Asteroids demo.
I am currently working on a new version of the engine using OpenGL. I've decided to switch to OpenGL, because I just don't have the time to write an OpenGL and DirectX renderer. I wanted the engine to be cross-platform, so I had to go with OpenGL. I've also switched over to the Bullet physics engine. The goals for this engine are:

* Scalable to lower-level hardware
* Portable to Linux/Mac/Windows
* Fractal terrain
* Destructible meshes
* Triangle mesh collision
* Simplified creation of game objects

The source code for this new version is available here.

## Features

* Renderer: Written from stratch in DirectX, heavy use of HLSL
* Physics: Using ODE. Automatic triangle mesh collision. Simple destructible meshes
* Persistence: Through XML using eXpat. All engine objects are XML-configurable
* Scripting: Using Lua. All engine objects can be manipulated by script
* Audio: Using FMOD, with 3D sound!
