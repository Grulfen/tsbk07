#version 150

out vec4 outColor;
in vec2 texCoord;
in vec3 outNormal;
uniform sampler2D tex;

vec3 normal;

void main(void)
{
        normal = normalize(outNormal);
        normal.x = abs(normal.x);
        normal.y = abs(normal.y);
        normal.z = abs(normal.z);

	//outColor = texture(tex, texCoord);
	outColor = vec4(normal, 1);
}
