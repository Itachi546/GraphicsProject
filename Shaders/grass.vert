#version 430
layout(location = 0)in vec2 position;
layout(location = 1)in vec3 translation;

out vec2 TexCoord;
out vec2 WorldPos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform float time;

void main(){
   mat4 translate = mat4(
     	  1, 0, 0, 0,
	  0, 1, 0, 0,
	  0, 0, 1, 0,
	  translation.x, translation.y +  model[1][1] * 0.5, translation.z, 1
     );

     mat4 m = translate * model;
     vec3 right = vec3(view[0][0], view[1][0], view[2][0]);
     vec3 up = vec3(0, 1, 0);

     vec3 pos  =  position.x * right + position.y * up;
     if(position.y > 0.1f){
         pos.x += sin(time * 10.0f+ float(gl_InstanceID * 10.0f)) * 0.2f;
        // pos.x += sin(time * 0.0008 + 10.0f) * 0.2f;
     }    

     vec4 worldPos = projection * view  * m * vec4(pos,   1.0f);

     gl_Position = worldPos;

     WorldPos = translation.xz;
     TexCoord = max(position, 0.0f) * 2.0f;
     TexCoord.y = 1.0f - TexCoord.y;
}