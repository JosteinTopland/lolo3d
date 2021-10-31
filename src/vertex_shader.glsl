attribute vec3 aPosition;
attribute vec3 aNormal;
attribute vec2 aTexcoord;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

varying vec3 vNormal;
varying vec2 vTexcoord;
varying vec3 vLightDirection;

void main()
{
    vTexcoord = aTexcoord;

    mat3 normalMatrix;
    normalMatrix[0] = normalize((viewMat * modelMat)[0].xyz);
    normalMatrix[1] = normalize((viewMat * modelMat)[1].xyz);
    normalMatrix[2] = normalize((viewMat * modelMat)[2].xyz);
    vNormal = normalize(normalMatrix * aNormal);

    vec4 v1 = viewMat * modelMat * vec4(aPosition, 1.0);
    vec3 v2 = v1.xyz / v1.w;
    vec3 lightPos = vec3(0.0, 0.0, 5.0);
    vLightDirection = normalize(lightPos - v2);

    gl_Position = projMat * viewMat * modelMat * vec4(aPosition, 1.0);
}