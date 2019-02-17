#version 430

in vec2 TexCoord;

out vec4 finalColor;
uniform sampler2D texture_diffuse;

void main(){
     vec2 texCoord = TexCoord;
     vec4 grassColor = texture(texture_diffuse,  texCoord) * vec4(1.0f, 1.0f, 0.0f, 1.0f);
     finalColor = grassColor;
}