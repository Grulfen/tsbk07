#version 150

in vec3 inPosition;
in vec3 inNormal;
out vec3 Color;
uniform mat4 myTransMat;
uniform mat4 myRotMatX;
uniform mat4 myRotMatY;
uniform mat4 myRotMatZ;

mat3 normalRotMatY = mat3(myRotMatY);

vec3 transformedNormal = normalRotMatY * inNormal;


void main(void)
{
        float shade;
        vec3 light = vec3(0.8, 0.8, -0.8);
        shade = dot(normalize(transformedNormal), light);
        Color = vec3(shade);
        
	//gl_Position = vec4(inPosition, 1.0);
	gl_Position = myTransMat * myRotMatZ * myRotMatY * myRotMatX * vec4(inPosition, 1.0);
}
