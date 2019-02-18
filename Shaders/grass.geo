#version 430

layout(points) in;
layout(triangle_strip) out;
layout(max_vertices = 12);

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec2 TexCoord;
out vec3 WorldPos;

mat4 rotationMatrix(vec3 axis, float angle)
{
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;
    
    return mat4(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
                0.0,                                0.0,                                0.0,                                1.0);
}
vec3 vLocalSeed;

// This function returns random number from zero to one
float randZeroOne()
{
    uint n = floatBitsToUint(vLocalSeed.y * 214013.0 + vLocalSeed.x * 2531011.0 + vLocalSeed.z * 141251.0);
    n = n * (n * n * 15731u + 789221u);
    n = (n >> 9u) | 0x3F800000u;
 
    float fRes =  2.0 - uintBitsToFloat(n);
    vLocalSeed = vec3(vLocalSeed.x + 147158.0 * fRes, vLocalSeed.y*fRes  + 415161.0 * fRes, vLocalSeed.z + 324154.0*fRes);
    return fRes;
}

int randomInt(int min, int max)
{
	float fRandomFloat = randZeroOne();
	return int(float(min)+fRandomFloat*float(max-min));
}

void main(){
     
     mat4 MV = view * model;
     mat4 MVP = projection * view * model;
     float radian = 3.14159 / 180.0;
     
     vec3 grassPos = gl_in[0].gl_Position.xyz;
     vec3 vBaseDir[] = {
     	  vec3(1.0, 0.0, 0.0),
	  vec3(float(cos(45.0*radian)), 0.0f, float(sin(45.0*radian))),
	  vec3(float(cos(-45.0*radian)), 0.0f, float(sin(-45.0*radian)))
     };

     float fGrassPatchSize = 5.0;

     for(int i = 0; i < 3; ++i){
     	     
	     vec3 rotatedDir = (rotationMatrix(vec3(0, 1, 0), 0.7) * vec4(vBaseDir[i], 1.0)).xyz;
	     vLocalSeed = grassPos * float(i);
	     int iGrassPatch = randomInt(0, 3);
	     float fGrassPatchHeight = 3.5 + randZeroOne() * 2.0;

	     float fTCStartX = float(iGrassPatch) * 0.25f;
	     float fTCEndX = fTCStartX + 0.25f;

	     vec3 vTL = grassPos - rotatedDir * fGrassPatchSize * 0.5f;
	     vTL.y += fGrassPatchHeight;
	     gl_Position = MVP * vec4(vTL, 1.0);
	     TexCoord = vec2(fTCStartX, 1.0);
	     WorldPos = vTL;
	     EmitVertex();

	     
		vec3 vBL = grassPos - vBaseDir[i]*fGrassPatchSize*0.5f;  
		gl_Position = MVP*vec4(vBL, 1.0);
		TexCoord = vec2(fTCStartX, 0.0);
		WorldPos = vBL;
		EmitVertex();

		vec3 vTR = grassPos + rotatedDir*fGrassPatchSize*0.5f;
		vTR.y += fGrassPatchHeight;  
		gl_Position = MVP*vec4(vTR, 1.0);
		TexCoord = vec2(fTCEndX, 1.0);
		WorldPos = vTR;
		EmitVertex();
		
		
		vec3 vBR = grassPos + vBaseDir[i]*fGrassPatchSize*0.5f;  
		gl_Position = MVP*vec4(vBR, 1.0);
		TexCoord = vec2(fTCEndX, 0.0);
		WorldPos = vBR;
		EmitVertex();
		
		EndPrimitive();	     
     }
}