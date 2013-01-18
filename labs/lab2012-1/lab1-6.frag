#version 150

out vec4 out_Color;
in vec3 Normal;

//in vec3 Color;

void main(void)
{
        out_Color = vec4(Normal, 1.0f);
        //out_Color = vec4(1.0f,0.7f,0.9f,1.0f);
}
