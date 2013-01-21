#version 150

in vec3 inPosition;
in vec3 inNormal;
out vec3 Normal;
//out vec3 Color;
uniform mat4 myTransMat;
uniform mat4 myRotMatX;
uniform mat4 myRotMatY;
uniform mat4 myRotMatZ;

mat3 normalRotMatY = mat3(myRotMatY);

    vec3 transformedNormal = normalRotMatY * inNormal;


void main(void)
{
        Normal = transformedNormal;
	//gl_Position = vec3(inPosition);
	gl_Position = myTransMat * myRotMatZ * myRotMatY * myRotMatX * vec4(inPosition, 1.0);
}
