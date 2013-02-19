#version 150

in vec2 outTexCoord;
in vec3 outPosition;
in vec3 outPositionCam;
in vec3 pixelPos;
in vec3 NormalTex;
out vec4 out_Color;

in vec3 Normal;

uniform sampler2D texUnit, texUnit2;
uniform bool skybox;
uniform mat4 camMatrix;

uniform vec3 lightSourcesDirPosArr[4];
uniform vec3 lightSourcesColorArr[4];
uniform float specularExponent[4];
uniform bool isDirectional[4];
uniform bool multitex;

vec3 reflection;

float cos_angle;

vec3 s;
vec3 n;
vec3 eye;
vec3 colors;
vec3 tex1;
vec3 tex2;
vec3 tmp_colors;

vec3 diffuse;
vec3 specular;

mat3 lightCamMatrix = mat3(camMatrix);

void main(void)
{
        colors  = vec3(0,0,0);
        if(multitex && NormalTex.x > 0){
                tmp_colors = vec3(texture(texUnit2, outTexCoord));
        } else {
                tmp_colors = vec3(texture(texUnit, outTexCoord));
        }
        colors += vec3(0.2,0.2,0.2);
        if(!skybox){
                //int i = 2;
                for(int i=0;i<4;i++){
                        if(isDirectional[i]){
                                 s = normalize(lightCamMatrix * lightSourcesDirPosArr[i]);
                        } else {
                                 s = normalize(lightCamMatrix * lightSourcesDirPosArr[i] - outPosition);
                        }
                        n = normalize(Normal);

                        // FIXME dett är fult och borde göras annorlunda
                        float lambert = dot(n, s)-0.001;
                        //float lambert = dot(n, s);

                        if(lambert > 0){
                                diffuse = (lightSourcesColorArr[i]*tmp_colors)*lambert;
                                colors += diffuse;

                                reflection = reflect(s, n);
                                eye = normalize(outPositionCam);

                                cos_angle = dot(reflection, eye);
                                cos_angle = max(0, cos_angle);
                                
                                specular = (lightSourcesColorArr[i]*tmp_colors)*pow(cos_angle, specularExponent[i]);
                                colors += specular;
                      }
                }
        } else { 
                colors = vec3(texture(texUnit, outTexCoord));
                //colors = tmp_colors;
        } 
        out_Color = vec4(colors, 1);
}
