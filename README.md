# model-duck
---
this is a tool I'm building for doing procedural 3D animations.

the main feature is the Moduloid and Duck Object. You can create a Duck Object and assign it an arbitrary amount of moduloids, which stack matrix transformations on the object. Duck Object also stores the parameters (as values or pointers) so you can have moduloids respond to other objects.

I'm not quite at the stage where I can document everything, but the main.c should have some example of some sort.

# building
---
**linux**
this is currently a "works on my machine" project.
but it shouldn't be too hard to figure out.

raylib has it's own makefile which needs to be run before the project is built.

if you have raylib installed to path it should also just work although i'd make sure to remove the path.
