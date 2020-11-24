cmake_minimum_required(VERSION 3.12)
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
project(MacTriangleuild)
# This is the name of the Exe change this and it will change everywhere
set(TargetName MacTriangle)
# find Qt libs
find_package(OpenGL REQUIRED)
# use C++ 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
# Set the name of the executable we want to build
add_executable(${TargetName})
include_directories(include)
# Add NGL include path
target_sources(${TargetName} PRIVATE 
${PROJECT_SOURCE_DIR}/src/main.mm  
${PROJECT_SOURCE_DIR}/src/GLFunctions.cpp 
${PROJECT_SOURCE_DIR}/src/GLWindow.cpp 
${PROJECT_SOURCE_DIR}/include/GLFunctions.h  
${PROJECT_SOURCE_DIR}/include/GLWindow.hpp  

)

# see what platform we are on and set platform defines
add_definitions(-DGL_SILENCE_DEPRECATION)
target_link_libraries(${TargetName} PRIVATE "-framework Metal")
target_link_libraries(${TargetName} PRIVATE "-framework AppKit")

# add exe and link libs that must be after the other defines
target_link_libraries(${TargetName} PRIVATE OpenGL::GL)
