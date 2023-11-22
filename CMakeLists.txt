cmake_minimum_required(VERSION 3.20)
#-------------------------------------------------------------------------------------------
# I'm going to use vcpk in most cases for our install of 3rd party libs
# this is going to check the environment variable for CMAKE_TOOLCHAIN_FILE and this must point to where
# vcpkg.cmake is in the University this is set in your .bash_profile to
# export CMAKE_TOOLCHAIN_FILE=/public/devel/2020/vcpkg/scripts/buildsystems/vcpkg.cmake
#-------------------------------------------------------------------------------------------
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE AND DEFINED ENV{CMAKE_TOOLCHAIN_FILE})
   set(CMAKE_TOOLCHAIN_FILE $ENV{CMAKE_TOOLCHAIN_FILE})
endif()

# Name of the project
project(ModernGLAll)
# for each subdirectory add the CMakeLists.txt file
add_subdirectory(${PROJECT_SOURCE_DIR}/GLFWTriangleGLEW)

if(NOT APPLE)
    add_subdirectory(${PROJECT_SOURCE_DIR}/GLUTTriangleGLEW)
endif()
if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    add_subdirectory(${PROJECT_SOURCE_DIR}/MacTriangle)
endif()
add_subdirectory(${PROJECT_SOURCE_DIR}/QtTriangleGLEW)
add_subdirectory(${PROJECT_SOURCE_DIR}/SDL2Triangle3DGLMGlew)
add_subdirectory(${PROJECT_SOURCE_DIR}/SDL2TriangleGLEW)
add_subdirectory(${PROJECT_SOURCE_DIR}/SFMLTriangle)
add_subdirectory(${PROJECT_SOURCE_DIR}/PointsGL3WSDL)

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    add_subdirectory(${PROJECT_SOURCE_DIR}/WindowsTriangleGLEW)
endif()
