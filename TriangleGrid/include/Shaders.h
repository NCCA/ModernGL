#ifndef SHADERS_H_
#define SHADERS_H_

#include <string>

const std::string vertex =R"(
#version 400 core
layout (location = 0) in vec3  inPosition;
layout (location = 1) in vec3  inNormal;
layout (location = 2) in vec2  inUV;

uniform mat4 MVP;
void main()
{
  gl_Position = MVP*vec4(inPosition, 1.0);
}
)";

 // some source for our fragment shader
const std::string fragment =R"(
#version 400 core
out vec4 fragColour;

void main()
{
  fragColour=vec4(1.0,1.0,1.0,1.0);
}
)";



#endif