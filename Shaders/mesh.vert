#version 430

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

out vec2 TexCoord;
out vec3 Normal;

out float visibility;
const float fogDensity = 0.007;
const float gradient = 1.5;

void main(){
     vec4 posRelativeToCam = view *  model * vec4(position.x, position.y  , position.z, 1.0);
     gl_Position = projection * posRelativeToCam;
     Normal = normal;
     TexCoord = texCoord;
    float distance = length(posRelativeToCam.xyz);
    visibility = clamp(pow(exp(-distance * fogDensity), gradient), 0.0f, 1.0f);
}