#version 150

out vec4 out_Color;
in vec3 Color;

void main(void)
{
        out_Color = vec4(Color, 1.0f);
        //out_Color = vec4(0.3f,0.0f,0.0f,1.0f);
}
