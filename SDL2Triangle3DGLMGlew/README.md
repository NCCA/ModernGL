# SDL2Triangle3DGLMGLEW

This program uses SDL2 to create a minimal OpenGL 4.x context and draw a 3D diffuse shaded triangle.

SDL 2 is expected to be installed using vcpkg or have the cmake package installed in the cmake search path.

OpenGL extensions are loaded using the GLEW library (which is not required under mac) and this is expected to be installed via [vcpkg](https://github.com/microsoft/vcpkg)

3D transformations are done using [GLM](https://github.com/g-truc/glm) using the lookAt and perspective functions to calculate the basic [Model View Project](http://www.opengl-tutorial.org/beginners-tutorials/tutorial-3-matrices/) matrix values

```
./vcpkg install GLEW
./vcpkg install SDL2
./vcpkg install glm
```

