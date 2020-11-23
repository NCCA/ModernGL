# QtTriangleGLEW

This program uses Qt5 and the [QOpenGLWindow](https://doc.qt.io/qt-5/qopenglwindow.html) to create a minimal OpenGL 4.x context and draw a triangle.

OpenGL extensions are loaded using the GLEW library (which is not required under mac) and this is expected to be installed via [vcpkg](https://github.com/microsoft/vcpkg)

```
./vcpkg install GLEW
```

The demo is built using cmake and you may need to specify the CMAKE_TOOLCHAIN_FILE to point to the correct location.