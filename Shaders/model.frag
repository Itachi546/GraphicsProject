#version 430

in vec2 TexCoords;
in vec3 Normal;

out vec4 finalColor;
in float visibility;
uniform sampler2D texture_diffuse;

void main() {

    vec3 skyColor = vec3(0.5, 0.6, 1.0);

    vec4 textureColor= texture(texture_diffuse, TexCoords);
    if(textureColor.a < 0.5) discard;
     vec3 n = normalize(Normal);
     vec3 l = normalize(vec3(1, 1, -1));
     float diffuse = max(dot(n, l), 0.1f);
     vec3 color = diffuse * textureColor.rgb;
     color = mix(skyColor, color, visibility);
    finalColor = vec4(color, textureColor.a);
}