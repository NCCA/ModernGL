# PointsGL3WSDL

This program uses SDL2 to create a minimal OpenGL 4.x context and draw a 3D diffuse shaded triangle. Extensions are loaded using gl3w


3D transformations are done using [GLM](https://github.com/g-truc/glm) using the lookAt and perspective functions to calculate the basic [Model View Project](http://www.opengl-tutorial.org/beginners-tutorials/tutorial-3-matrices/) matrix values

```
./vcpkg install gl3w
./vcpkg install SDL2
./vcpkg install glm
```

