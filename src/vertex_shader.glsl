#version 410 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexcoord;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

out vec3 vLightDirection;
out vec3 vNormal;
out vec2 vTexcoord;

void main()
{
    vec4 v1 = viewMat * modelMat * vec4(aPosition, 1.0);
    vec3 v2 = v1.xyz / v1.w;
    vec3 lightPos = vec3(0.0, 0.0, 5.0);
    vLightDirection = normalize(lightPos - v2);

    mat3 normalMatrix;
    normalMatrix[0] = normalize((viewMat * modelMat)[0].xyz);
    normalMatrix[1] = normalize((viewMat * modelMat)[1].xyz);
    normalMatrix[2] = normalize((viewMat * modelMat)[2].xyz);
    vNormal = normalize(normalMatrix * aNormal);

    vTexcoord = aTexcoord;

    gl_Position = projMat * viewMat * modelMat * vec4(aPosition, 1.0);
}
