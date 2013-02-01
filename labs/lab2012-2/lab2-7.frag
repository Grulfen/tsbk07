#version 150

in vec2 outTexCoord;
out vec4 out_Color;

in vec3 Normal;

uniform sampler2D texUnit;


void main(void)
{
        const vec3 light = vec3(0.58, 0.58, 0.58);
        out_Color = texture(texUnit, outTexCoord)*dot(light, normalize(Normal));
}
