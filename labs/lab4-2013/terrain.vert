#version 150

in  vec3 inPosition;
in  vec3 inNormal;
in  vec2 inTexCoord;

out vec2 texCoord;
out vec3 outNormal;
out vec3 outPosition;
out vec3 outPositionCam;

// NY
uniform mat4 projMatrix;
uniform mat4 mdlMatrix;

mat3 normalMatrix;

void main(void)
{
	normalMatrix = mat3(mdlMatrix);
	outNormal = normalMatrix * inNormal;
	texCoord = inTexCoord;
	gl_Position = projMatrix * mdlMatrix * vec4(inPosition, 1.0);
        outPosition = normalMatrix * inPosition;
        outPositionCam = vec3(mdlMatrix * vec4(inPosition, 1));
}
