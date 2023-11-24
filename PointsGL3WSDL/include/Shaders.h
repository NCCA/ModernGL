#ifndef SHADERS_H__
#define SHADERS_H__

#include <string>

const std::string vertex =R"(
#version 400 core
layout (location = 0) in vec3  inPosition;
layout (location = 1) in vec3 inColour;
uniform mat4 MVP;
out vec3 vertColour;
out vec3 fragPos;
void main()
{
  gl_Position = MVP*vec4(inPosition, 1.0);
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
  fragColour.rgb=vertColour;
}
)";



#endif