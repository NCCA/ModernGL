#ifndef SHADERS_H__
#define SHADERS_H__

#include <string>

const std::string vertex =R"(
#version 400 core
layout (location = 0) in vec3  inPosition;
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