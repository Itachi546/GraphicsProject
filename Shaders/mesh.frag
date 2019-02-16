#version 430

in vec3 Normal;

out vec4 fragColor;
in vec2 TexCoord;

uniform sampler2D splatmap;
uniform sampler2D grassTexture;
uniform sampler2D rockTexture;
uniform sampler2D dirtTexture;

float tileSize = 10.0f;
void main(){
     vec4 splatColor = texture(splatmap, TexCoord);
     vec2 tiledTexCoord = TexCoord * tileSize;
     vec3 grassColor = splatColor.g * texture(grassTexture, tiledTexCoord).rgb; 
     vec3 dirtColor  = splatColor.r * texture(dirtTexture, tiledTexCoord).rgb;
     vec3 rockColor  = splatColor.b * texture(rockTexture, tiledTexCoord).rgb;

     vec3 n = normalize(Normal);
     vec3 l = normalize(vec3(1, 1, 0));

     float diffuse = max(dot(n, l), 0.1f);

     vec3 color = diffuse * (grassColor + dirtColor + rockColor);
     fragColor = vec4(color, 1.0f);
}