#version 150

in vec3 inPosition;

uniform mat4 projectionMatrix;
uniform mat4 mdlMatrix;

in vec2 inTexCoord;
out vec2 outTexCoord;


void main(void)
{
        //Normal = transformedNormal;
	gl_Position = projectionMatrix * mdlMatrix * (vec4(inPosition, 1.0));
        outTexCoord = inTexCoord;
}
