#ifndef SHADERS_H__
#define SHADERS_H__

#include <string>


#include <string>

const std::string vertex =R"(
#version 400 core

layout (location = 0) in vec3  inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2  inUV;

uniform mat4 MVP;
uniform mat4 model;
out vec3 normal;
out vec3 fragPos;
void main()
{
  gl_Position = MVP*vec4(inPosition, 1.0);
  normal = normalize(inNormal);
  fragPos = vec3(model * vec4(inPosition, 1.0));
}
)";

 // some source for our fragment shader
  const std::string fragment =R"(
#version 400 core
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
	vec3 diffuse = vec3(1.0,1.0,1.0) * sDotN;
  fragColour = vec4(diffuse,1.0);
}
)";




const std::string colourvertex =R"(
#version 400 core

layout (location = 0) in vec3  inPosition;

uniform mat4 MVP;

void main()
{
  gl_Position = MVP*vec4(inPosition, 1.0);
}
)";

 // some source for our fragment shader
  const std::string colourfragment =R"(
#version 400 core
out vec4 fragColour;

void main()
{
  fragColour = vec4(1.0);
}
)";


#endif