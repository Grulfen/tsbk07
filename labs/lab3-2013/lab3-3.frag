#version 150

//in vec2 outTexCoord;
out vec4 out_Color;

in vec3 Normal;

uniform sampler2D texUnit;
uniform vec2 coords;

void main(void)
{
        const vec3 light = vec3(0.58, 0.58, 0.58);
        out_Color = vec4(vec3(coords/255,0.8)*dot(light, normalize(Normal)), 1);
        //out_Color = texture(texUnit, outTexCoord)*dot(light, normalize(Normal));
}
