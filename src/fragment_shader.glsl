#version 410 core

uniform sampler2D uSampler;
uniform bool uEnableTexture;
uniform vec4 uDiffuseColor;

in vec3 vLightDirection;
in vec3 vNormal;
in vec2 vTexcoord;

layout (location = 0) out vec4 color;

void main()
{
    float intensity = max(0.1, dot(vNormal, vLightDirection));

    if (uEnableTexture) {
        color = texture(uSampler, vTexcoord);
    } else {
        color = uDiffuseColor;
    }

    color *= intensity * 1.5;
}