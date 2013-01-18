#version 150

in vec3 in_Position;
in vec3 in_Color;
out vec3 Color;
uniform mat4 myTransMat;
uniform mat4 myRotMatX;
uniform mat4 myRotMatY;
uniform mat4 myRotMatZ;

void main(void)
{
	//gl_Position = myTransMat * myRotMat * vec4(in_Position, 1.0);
	gl_Position = myRotMatZ * myRotMatY * myRotMatX * vec4(in_Position, 1.0);
	//gl_Position = vec4(in_Position, 1.0);
        Color = in_Color;
}
