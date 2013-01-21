#version 150

out vec4 out_Color;

in vec3 Color;

void main(void)
{
        out_Color = vec4(Color, 1.0);
}
