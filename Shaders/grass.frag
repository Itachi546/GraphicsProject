#version 430

in vec2 TexCoord;
in vec2 WorldPos;

out vec4 finalColor;

uniform sampler2D texture_diffuse;

float rand(vec2 co){
    return max(fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453), 0.4);
}

void main(){
     vec4 grassColor = texture(texture_diffuse,  TexCoord) * vec4(rand(WorldPos), rand(WorldPos), 0.0f, 1.0f);
     if(grassColor.a < 0.5) discard;
     finalColor = grassColor;
}