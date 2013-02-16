#version 150

in vec2 outTexCoord;
in vec3 outPosition;
out vec4 out_Color;

in vec3 Normal;

uniform sampler2D texUnit;
uniform int skybox;
uniform mat4 camMatrix;

uniform vec3 lightSourcesDirPosArr[4];
uniform vec3 lightSourcesColorArr[4];
uniform float specularExponent[4];
uniform bool isDirectional[4];

vec3 reflection;

vec3 colors;

mat3 lightCamMatrix = mat3(camMatrix);

void main(void)
{
        if(!skybox){
                //reflection = 2*Normal*dot(lightSourcesDirPosArr[0], Normal) - lightSourcesDirPosArr[0];
                colors = vec3(texture(texUnit, outTexCoord));
                colors = (colors*lightSourcesColorArr[0])*dot(normalize(lightCamMatrix*lightSourcesDirPosArr[0]), normalize(Normal));
        } else {
                colors = vec3(texture(texUnit, outTexCoord));
        }
        out_Color = vec4(colors, 1);
}
