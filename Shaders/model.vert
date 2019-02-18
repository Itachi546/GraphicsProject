#version 430

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;
layout(location = 3) in vec3 worldPos;

out vec2 TexCoords;
out vec3 Normal;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

out float visibility;
const float fogDensity = 0.007;
const float gradient = 1.5;

void main() {
     mat4 translate = mat4(
          1, 0, 0, 0,
          0, 1, 0, 0,
          0, 0, 1, 0,
     	worldPos.x, worldPos.y, worldPos.z, 1
     );

   vec4 posRelativeToCam = view * translate * model  * vec4(position, 1.0f);
   gl_Position = projection * posRelativeToCam;
   TexCoords = texCoords;
   Normal = normal;

   float distance = length(posRelativeToCam.xyz);
   visibility = clamp(pow(exp(-distance * fogDensity), gradient), 0.0f, 1.0f);
}