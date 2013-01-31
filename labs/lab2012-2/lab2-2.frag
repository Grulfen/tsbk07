#version 150

in vec2 outTexCoord;
out vec4 out_Color;

in float sin_out;
//in vec3 Normal;

uniform sampler2D texUnit;

void main(void)
{
        //const vec3 light = vec3(-0.5, 0.6, -0.6);
        //float shade;
        //shade = dot(normalize(Normal), light);

        out_Color = texture(texUnit, outTexCoord);
        //out_Color = vec4(abs(sin_out), abs(sin_out), abs(sin_out), 1.0);
}
