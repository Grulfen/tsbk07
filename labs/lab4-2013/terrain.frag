#version 150

out vec4 outColor;
in vec2 texCoord;
in vec3 outNormal;
uniform sampler2D tex;

void main(void)
{
	//outColor = texture(tex, texCoord);
	outColor = vec4(normalize(outNormal), 1);
}
