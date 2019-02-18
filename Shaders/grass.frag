
#version 430

in vec2 TexCoord;
in vec2 WorldPos;
in float visibility;

out vec4 finalColor;

uniform sampler2D grass;
uniform sampler2D fern;
uniform sampler2D flower;

float rand(float n){return fract(sin(n) * 43758.5453123);}
/*

void main(){
     vec4 grassColor = texture(texture_diffuse,  TexCoord) * vec4(rand(WorldPos), rand(WorldPos), 0.0f, 1.0f);
     if(grassColor.a < 0.5) discard;
     finalColor = vec4(grassColor.rgb, 1.0f);
}

*/

void main(){
    vec4 color = vec4(0.0f);
    float r = rand(pow(length(WorldPos), 5.0f));
    vec3 skyColor = vec3(0.5, 0.6, 1.0);
/*
    if(r > 0.4f)
    	 color = texture(grass, TexCoord);
    else if(r < 0.2f)
    	 color = texture(flower, TexCoord);
    else
	color = texture(fern, TexCoord);
*/
    if(r > 0.3f)
    	 color = texture(grass, TexCoord);
    else
    	 color = texture(flower, TexCoord);

     if(color.a < 0.5) discard;
     color.rgb =  mix(skyColor, color.rgb, visibility);
     finalColor = color;
}