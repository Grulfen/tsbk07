#version 150

in vec3 in_Position;
in vec3 in_Color;
out vec3 Color;
uniform mat4 myTransMat;
uniform mat4 myRotMat;

void main(void)
{
	//gl_Position = myTransMat * myRotMat * vec4(in_Position, 1.0);
	gl_Position = myTransMat * myRotMat * vec4(in_Position, 1.0);
        Color = in_Color;
}
