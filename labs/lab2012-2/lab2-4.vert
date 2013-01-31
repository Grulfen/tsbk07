#version 150

in vec3 inPosition;

uniform mat4 projectionMatrix;
uniform mat4 mdlMatrix;
uniform mat4 camMatrix;

in vec2 inTexCoord;
out vec2 outTexCoord;


void main(void)
{
        //Normal = transformedNormal;
	gl_Position = projectionMatrix * camMatrix * mdlMatrix * (vec4(inPosition, 1.0));
        outTexCoord = inTexCoord;
}
