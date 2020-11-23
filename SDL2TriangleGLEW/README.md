# SDL2TriangleGLEW

This program uses SDL2 to create a minimal OpenGL 4.x context and draw a triangle.

SDL 2 is expected to be installed using vcpkg or have the cmake package installed in the cmake search path.

OpenGL extensions are loaded using the GLEW library (which is not required under mac) and this is expected to be installed via [vcpkg](https://github.com/microsoft/vcpkg)

```
./vcpkg install GLEW
```

The demo is built using cmake and you may need to specify the CMAKE_TOOLCHAIN_FILE to point to the correct location.