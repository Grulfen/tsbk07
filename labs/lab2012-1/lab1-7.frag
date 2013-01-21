#version 150

out vec4 out_Color;
in vec3 Normal;

//in vec3 Color;

void main(void)
{
        const vec3 light = vec3(-0.5, 0.6, -0.6);
        float shade;
        vec3 color_pink = vec3(1.0, 0.6, 0.8);

        shade = dot(normalize(Normal), light);
        out_Color = vec4(shade*color_pink[0], shade*color_pink[1], shade*color_pink[2], 1.0);
}
