#version 430

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

out vec3 Normal;

void main(){
     gl_Position = projection * view *  model * vec4(position.x, position.y  , position.z, 1.0);
     Normal = normal;
}