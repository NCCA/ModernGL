#ifndef SHADERS_H__
#define SHADERS_H__

#include <string>

const std::string vertex =R"(
#version 400 core

layout (location = 0) in vec3  inPosition;
layout (location = 1) in vec3 inColour;
layout (location = 2) in vec3 inNormal;
uniform mat4 MVP;
uniform mat4 model;
out vec3 vertColour;
out vec3 normal;
out vec3 fragPos;
void main()
{
  gl_Position = MVP*vec4(inPosition, 1.0);
  vertColour = inColour;
  normal = normalize(inNormal);
  fragPos = vec3(model * vec4(inPosition, 1.0));
}
)";

 // some source for our fragment shader
  const std::string fragment =R"(
#version 400 core
in vec3 vertColour;
in vec3 normal;
in vec3 fragPos;
out vec4 fragColour;
uniform vec3 lightPos;  

void main()
{
	vec3 s = normalize(lightPos - fragPos);
	vec3 v = normalize(-fragPos);
	vec3 h = normalize( v + s );
	float sDotN = max( dot(s,normal), 0.0 );
	vec3 diffuse = vertColour * sDotN;
  fragColour = vec4(diffuse,1.0);
}
)";


#endif