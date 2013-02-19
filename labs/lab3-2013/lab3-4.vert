#version 150

in vec3 inPosition;
in vec3 inNormal;
in vec2 inTexCoord;

out vec3 outPosition;
out vec3 outPositionCam;
out vec3 Normal;
out vec3 NormalTex;
out vec2 outTexCoord;
out vec3 pixelPos;

uniform mat4 projectionMatrix;
uniform mat4 mdlMatrix;
uniform mat4 camMatrix;

mat3 normalMatrix = mat3(camMatrix * mdlMatrix);
vec3 transformedNormal = normalMatrix * inNormal;

void main(void)
{
        pixelPos = inPosition;
        NormalTex = inNormal;
        Normal = transformedNormal;
        gl_Position = projectionMatrix * camMatrix * mdlMatrix * vec4(inPosition, 1);
        outTexCoord = inTexCoord;
        outPositionCam = vec3(camMatrix * mdlMatrix * vec4(inPosition, 1));
        outPosition = mat3(camMatrix) * vec3(mdlMatrix * vec4(inPosition, 1));
}
