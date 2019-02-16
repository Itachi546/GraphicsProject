#version 430

in vec3 Normal;

out vec4 fragColor;

void main(){
//     fragColor = vec4(vec3(1.0f), 1.0f);
     vec3 n = normalize(Normal);
     vec3 l = normalize(vec3(1, 1, 0));

     float diffuse = max(dot(n, l), 0.1f);

     vec3 color = diffuse * vec3(1.0f);
     color /= (color + 1.0f);
     color = pow(color, vec3(1/2.2f));
     fragColor = vec4(color, 1.0f);
}