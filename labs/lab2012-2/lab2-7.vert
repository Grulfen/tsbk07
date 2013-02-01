#version 150

in vec3 inPosition;
in vec3 inNormal;
in vec2 inTexCoord;

out vec3 Normal;
out vec2 outTexCoord;

uniform mat4 projectionMatrix;
uniform mat4 mdlMatrix;
uniform mat4 camMatrix;

mat3 normalMatrix = mat3(mdlMatrix);
vec3 transformedNormal = normalMatrix * inNormal;

void main(void)
{
        Normal = transformedNormal;
	gl_Position = projectionMatrix * camMatrix * mdlMatrix * (vec4(inPosition, 1.0));
        outTexCoord = inTexCoord;
}
