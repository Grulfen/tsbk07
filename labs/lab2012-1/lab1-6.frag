#version 150

out vec4 out_Color;
in vec3 Normal;

//in vec3 Color;

void main(void)
{
        float x = abs(Normal[0]);
        //out_Color = vec4(Normal, 1.0f);
        out_Color = vec4(x,x,x,1.0f);
}
