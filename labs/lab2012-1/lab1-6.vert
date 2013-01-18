#version 150

in vec3 inPosition;
in vec3 inNormal;
out vec3 Normal;
//out vec3 Color;
uniform mat4 myTransMat;
uniform mat4 myRotMatX;
uniform mat4 myRotMatY;
uniform mat4 myRotMatZ;

void main(void)
{
	//gl_Position = vec4(inPosition, 1.0);
	gl_Position = myRotMatZ * myRotMatY * myRotMatX * vec4(inPosition, 1.0);
        Normal = inNormal;
}
