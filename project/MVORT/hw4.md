* What I wanted to do by this deadline:

By this deadline I wanted to have the mesh rendering finished, with object/scene import.

* What I achieved

This feature took a _lot_ of work, but I managed to (mostly) complete it.  Right now the raytracer can import many (40+) mesh, and scene formats, with command line options.  However, this feature is not as complete as I want it to be.  For instance, you cannot instance the meshes.  Completing this feature will require me to bring in a linear algebra library (eigen3) for matrix transformations; this will also require a rewrite of all vector math to use the library (replacing vec3.h, vec3.cpp).  The asset import library that I am using (assimp) also supports lights, camera settings, and textures/materials which I am not making use of (rather than just triangle data).  Finally, the scene data itself (i.e. placement of spheres, object attributes) is still hard-coded.  I have a pretty good idea of how this data can be serialized (language schema), and plan on using a sort of binary format using either a JSON library (slow), or a binary format via Flatbuffers/Cap'n Proto library (fast).  The reason I did not do this feature yet is that I would need to write UI code to interactively define a scene to save/load.  This feature would not make sense to write before making the GUI portion.

* What I will do by next deadline

Do the GUI!  This should (hopefully) include viewing the image as it is being rendered (QPainter?), and the aformentioned scene definition, saving, and loading from file.

* Images.

UV mapping for textures on a sphere:
![](https://i.imgur.com/Xt1GbEM.png)

Lighting update with placable lights:
![](https://i.imgur.com/bSJYV6j.jpg)

Traingles:


![](https://i.imgur.com/q0jH6q5.png)

Model import:
![](https://i.imgur.com/8ejTvSL.png)
