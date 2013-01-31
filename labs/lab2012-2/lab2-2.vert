#version 150

in vec3 inPosition;

// in vec3 inNormal;
// out vec3 Normal;
// out vec3 Color;

uniform mat4 myTransMat;
uniform mat4 myRotMatX;
uniform mat4 myRotMatY;
uniform mat4 myRotMatZ;

in vec2 inTexCoord;
out vec2 outTexCoord;

float sin_t = myRotMatY[0][2];
//float cos_t = myRotMatY[0][0];

float sin_3t = myRotMatX[1][1];
float sin_2t = myRotMatZ[0][0];

//mat3 normalRotMatY = mat3(myRotMatY);
//vec3 transformedNormal = normalRotMatY * inNormal;


out float sin_out;


void main(void)
{
        //Normal = transformedNormal;
	//gl_Position = vec3(inPosition);
        sin_out = sin_3t;
        outTexCoord = inTexCoord;

	gl_Position = myTransMat * myRotMatY * vec4(inPosition, 1.0);
}
