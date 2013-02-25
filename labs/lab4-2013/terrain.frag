#version 150

in vec2 texCoord;
in vec3 outNormal;
in vec3 outPosition;
in vec3 outPositionCam;

out vec4 outColor;

uniform sampler2D tex;
uniform vec3 lightSourcesDirPosArr;
uniform vec3 lightSourcesColorArr;
uniform float specularExponent;
uniform bool isDirectional;
uniform mat4 mdlMatrix;
uniform bool skybox;

vec3 s;
vec3 n;
mat3 mdlmat3 = mat3(mdlMatrix);

vec3 colors;
vec3 tex_colors;
vec3 diffuse, specular, reflection, eye;

float cos_angle;

void main(void)
{
        colors = vec3(0,0,0);
        tex_colors = vec3(texture(tex, texCoord));
        if(!skybox){
                if(isDirectional){
                        s = normalize(mdlmat3 * lightSourcesDirPosArr);
                } else {
                        s = normalize(outPosition - mdlmat3 * lightSourcesDirPosArr);
                }
                n = normalize(outNormal);
                float lambert = dot(s,n);
                if (lambert>0){
                        diffuse = (lightSourcesColorArr*tex_colors)*lambert;
                        colors += diffuse;

                        reflection = reflect(s, n);
                        eye = normalize(outPositionCam);

                        cos_angle = max(0, dot(reflection, eye));

                        specular = (lightSourcesColorArr*tex_colors)*pow(cos_angle, specularExponent);
                        colors += specular;
                }
        } else {
                colors = tex_colors; 
        }
        outColor = vec4(colors, 1);
}
