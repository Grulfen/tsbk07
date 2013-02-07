#version 150

in vec3 inPosition;
in vec3 inNormal;
in vec2 inTexCoord;

out vec3 Normal;
out vec2 outTexCoord;

uniform mat4 projectionMatrix;
uniform mat4 mdlMatrix;
uniform mat4 camMatrix;

uniform float t;

mat3 normalMatrix = mat3(mdlMatrix);
vec3 transformedNormal = normalMatrix * inNormal;

vec3 transformedPosition;

void main(void)
{
        transformedPosition = inPosition;
        transformedPosition[2] = 1*transformedPosition[2];
        if(transformedPosition[2] < -0.4){
                transformedPosition[1] = transformedPosition[1] + 0.2*sin(10*transformedPosition[2]);
                transformedPosition[2] = 0.8*transformedPosition[2];
        }
        //transformedPosition[1] = transformedPosition[1] + 0.1*(transformedPosition[2]+2)*sin(5*transformedPosition[2] + 0.01*t);
        //transformedPosition[1] = transformedPosition[1] + 0.2*sin(4*transformedPosition[2]);
        //transformedPosition[0] = transformedPosition[0] + 0.2*cos(4*transformedPosition[2]);
        Normal = transformedNormal;
        gl_Position = projectionMatrix * camMatrix * mdlMatrix * (vec4(transformedPosition, 1.0));
        outTexCoord = inTexCoord;
}
