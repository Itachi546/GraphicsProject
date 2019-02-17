#version 430
layout(location = 0)in vec2 position;
layout(location = 1)in vec3 translation;

out vec2 TexCoord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main(){
     vec3 pos = vec3(model * vec4(position, 0.0, 1.0)) + translation; 
     gl_Position = projection * view * vec4(pos, 1.0);
     TexCoord = vec2(position.x, 1 - position.y);
}