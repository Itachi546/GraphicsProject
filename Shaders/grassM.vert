#version 430

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in vec3 worldPos;

uniform mat4 model, view, projection;
uniform float time;
out vec2  TexCoord;
out vec2 WorldPos;

out float visibility;
const float fogDensity = 0.007;
const float gradient = 1.5;

void main(){
     mat4 translate = mat4(
     1, 0, 0, 0,
     0, 1, 0, 0,
     0, 0, 1, 0,
     worldPos.x, worldPos.y, worldPos.z, 1
     );

     float frequency = 30.0f;

     vec3 pos = position;
     if(pos.y > 1.0f){
         pos.x += sin(time * frequency + float(gl_InstanceID * 10.0f));
         pos.z += sin(time * frequency + float(gl_InstanceID * 10.0f) + 50.0f);
    }	 

     mat4 m = translate * model;
     vec4 posRelativeToCam =  view * m * vec4(pos, 1.0f);     
     gl_Position = projection * posRelativeToCam;
     TexCoord = texCoord;
     WorldPos = worldPos.xz;

   float distance = length(posRelativeToCam.xyz);
   visibility = clamp(pow(exp(-distance * fogDensity), gradient), 0.0f, 1.0f);
  }