// Sample code showing how to create a modern OpenGL window and rendering context on Win32.
// taken from https://gist.github.com/nickrolfe/1127313ed1dbf80254b614a721b3ee9c
// could do with some updating to latest API's 
#include <windows.h>
#include <gl/glew.h>
#include <gl/wglew.h>
#include <string>
#include <iostream>
#include "GLFunctions.h"

typedef HGLRC WINAPI wglCreateContextAttribsARB_type(HDC hdc, HGLRC hShareContext,const int *attribList);
wglCreateContextAttribsARB_type *wglCreateContextAttribsARB;


typedef BOOL WINAPI wglChoosePixelFormatARB_type(HDC hdc, const int *piAttribIList,
        const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats);
wglChoosePixelFormatARB_type *wglChoosePixelFormatARB;

static void fatal_error(const std::string &msg)
{
    MessageBoxA(NULL, msg.c_str(), "Error", MB_OK | MB_ICONEXCLAMATION);
    exit(EXIT_FAILURE);
}

static void init_opengl_extensions(void)
{
    // Before we can load extensions, we need a dummy OpenGL context, created using a dummy window.
    // We use a dummy window because you can only set the pixel format for a window once. For the
    // real window, we want to use wglChoosePixelFormatARB (so we can potentially specify options
    // that aren't available in PIXELFORMATDESCRIPTOR), but we can't load and use that before we
    // have a context.
    WNDCLASSA window_class = 
    {
        .style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC,
        .lpfnWndProc = DefWindowProcA,
        .hInstance = GetModuleHandle(0),
        .lpszClassName = "Dummy_WGL_djuasiodwa",
    };

    if (!RegisterClassA(&window_class)) 
    {
        fatal_error("Failed to register dummy OpenGL window.");
    }

    HWND dummy_window = CreateWindowExA(
        0,
        window_class.lpszClassName,
        "Dummy OpenGL Window",
        0,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        0,
        0,
        window_class.hInstance,
        0);

    if (!dummy_window) 
    {
        fatal_error("Failed to create dummy OpenGL window.");
    }

    HDC dummy_dc = GetDC(dummy_window);

    PIXELFORMATDESCRIPTOR pfd; 
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.cColorBits = 32;
    pfd.cAlphaBits = 8;
    pfd.iLayerType = PFD_MAIN_PLANE;
    pfd.cDepthBits = 24;
    pfd.cStencilBits = 8;

    int pixel_format = ChoosePixelFormat(dummy_dc, &pfd);
    if (!pixel_format) 
    {
        fatal_error("Failed to find a suitable pixel format.");
    }
    if (!SetPixelFormat(dummy_dc, pixel_format, &pfd)) 
    {
        fatal_error("Failed to set the pixel format.");
    }

    HGLRC dummy_context = wglCreateContext(dummy_dc);
    if (!dummy_context) 
    {
        fatal_error("Failed to create a dummy OpenGL rendering context.");
    }

    if (!wglMakeCurrent(dummy_dc, dummy_context)) 
    {
        fatal_error("Failed to activate dummy OpenGL rendering context.");
    }

    wglCreateContextAttribsARB = (wglCreateContextAttribsARB_type*)wglGetProcAddress(
        "wglCreateContextAttribsARB");
    wglChoosePixelFormatARB = (wglChoosePixelFormatARB_type*)wglGetProcAddress(
        "wglChoosePixelFormatARB");

    wglMakeCurrent(dummy_dc, 0);
    wglDeleteContext(dummy_context);
    ReleaseDC(dummy_window, dummy_dc);
    DestroyWindow(dummy_window);
}

static HGLRC init_opengl(HDC real_dc)
{
    init_opengl_extensions();

    // Now we can choose a pixel format the modern way, using wglChoosePixelFormatARB.
    int pixel_format_attribs[] = 
    {
        WGL_DRAW_TO_WINDOW_ARB,     GL_TRUE,
        WGL_SUPPORT_OPENGL_ARB,     GL_TRUE,
        WGL_DOUBLE_BUFFER_ARB,      GL_TRUE,
        WGL_ACCELERATION_ARB,       WGL_FULL_ACCELERATION_ARB,
        WGL_PIXEL_TYPE_ARB,         WGL_TYPE_RGBA_ARB,
        WGL_COLOR_BITS_ARB,         32,
        WGL_DEPTH_BITS_ARB,         24,
        WGL_STENCIL_BITS_ARB,       8,
        0
    };

    int pixel_format;
    UINT num_formats;
    wglChoosePixelFormatARB(real_dc, pixel_format_attribs, 0, 1, &pixel_format, &num_formats);
    if (!num_formats) 
    {
        fatal_error("Failed to set the OpenGL 3.3 pixel format.");
    }

    PIXELFORMATDESCRIPTOR pfd;
    DescribePixelFormat(real_dc, pixel_format, sizeof(pfd), &pfd);
    if (!SetPixelFormat(real_dc, pixel_format, &pfd)) 
    {
        fatal_error("Failed to set the OpenGL 3.3 pixel format.");
    }

    // Specify that we want to create an OpenGL 3.3 core profile context
    int gl33_attribs[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
        WGL_CONTEXT_MINOR_VERSION_ARB, 5,
        WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0,
    };

    HGLRC gl33_context = wglCreateContextAttribsARB(real_dc, 0, gl33_attribs);
    if (!gl33_context) {
        fatal_error("Failed to create OpenGL 3.3 context.");
    }

    if (!wglMakeCurrent(real_dc, gl33_context)) {
        fatal_error("Failed to activate OpenGL 3.3 rendering context.");
    }

    return gl33_context;
}

static LRESULT CALLBACK
window_callback(HWND window, UINT msg, WPARAM wparam, LPARAM lparam)
{
    LRESULT result = 0;

    switch (msg) {
        case WM_CLOSE:
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            result = DefWindowProcA(window, msg, wparam, lparam);
            break;
    }

    return result;
}

static HWND
create_window(HINSTANCE inst)
{

/*



*/


    WNDCLASSA window_class = {
        .style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC,
        .lpfnWndProc = window_callback,
        .hInstance = inst,
        .hCursor = LoadCursor(0, IDC_ARROW),
        .hbrBackground = 0,
        .lpszClassName = "WGL_fdjhsklf",
    };

    if (!RegisterClassA(&window_class)) {
        fatal_error("Failed to register window.");
    }

    // Specify a desired width and height, then adjust the rect so the window's client area will be
    // that size.
    RECT rect = {
        .right = 1024,
        .bottom = 576,
    };
    DWORD window_style = WS_OVERLAPPEDWINDOW;
    AdjustWindowRect(&rect, window_style, false);

    HWND window = CreateWindowExA(
        0,
        window_class.lpszClassName,
        "OpenGL",
        window_style,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        rect.right - rect.left,
        rect.bottom - rect.top,
        0,
        0,
        inst,
        0);

    if (!window) {
        fatal_error("Failed to create window.");
    }

    return window;
}

int WINAPI
WinMain(HINSTANCE inst, HINSTANCE prev, LPSTR cmd_line, int show)
{
    HWND window = create_window(inst);
    HDC gldc = GetDC(window);
    HGLRC glrc = init_opengl(gldc);


    ShowWindow(window, show);
    UpdateWindow(window);

    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        std::cerr<<"Error: "<<glewGetErrorString(err)<<'\n';
        exit(EXIT_FAILURE);
    }
    std::cerr<<"Status: Using GLEW"<< glewGetString(GLEW_VERSION)<<'\n';

    // create the Quad
    auto vaoID=createTriangle(0.8f);
    const std::string vertex =R"(
    #version 400 core

    layout (location = 0) in vec3  inPosition;
    layout (location = 1) in vec3 inColour;
    out vec3 vertColour;
    void main()
    {
    gl_Position = vec4(inPosition, 1.0);
    vertColour = inColour;
    }
    )";

    // some source for our fragment shader
    const std::string fragment =R"(
    #version 400 core
    in vec3 vertColour;
    out vec4 fragColour;
    void main()
    {
    fragColour = vec4(vertColour,1.0);
    }
    )";

    auto shaderID=loadShaderFromStrings(vertex,fragment);

    bool running = true;
    while (running) 
    {
      MSG msg;
      while (PeekMessageA(&msg, 0, 0, 0, PM_REMOVE)) 
      {
        if (msg.message == WM_QUIT) 
        {
          running = false;
        } 
        else 
        {
          TranslateMessage(&msg);
          DispatchMessageA(&msg);
        }
      }
    glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Do OpenGL rendering here
    glBindVertexArray(vaoID);		// select first bind the array
    glDrawArrays(GL_TRIANGLES, 0, 3);	// draw object

    SwapBuffers(gldc);
  }

   return EXIT_SUCCESS;
}