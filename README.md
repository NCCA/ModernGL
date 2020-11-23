# Minimal OpenGL Demos

![](ModernGL.png)

These demos will load a simple shader and draw a triangle using Modern OpenGL. Each demo will use different methods to generate the OpenGL Context and load the extensions. 

All extension loading libraries are assumed to be installed via vcpkg.

## Demos

[**QtTriangleGLEW**](QtTriangleGLEW) :- use Qt to create and OpenGL context and Glew to load extensions. This will work under mac, windows and linux as long as Qt and GLEW are installed.

[**WindowsTriangleGLEW**](WindowsTriangleGLEW) :- use Win32 to create a window and glew to load extensions. This is based on [this](https://gist.github.com/nickrolfe/1127313ed1dbf80254b614a721b3ee9c) gist for windows creation and could do with updating.
