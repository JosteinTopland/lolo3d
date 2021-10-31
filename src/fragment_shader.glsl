uniform sampler2D uSampler;
uniform bool uEnableTexture;

varying vec3 vNormal;
varying vec2 vTexcoord;
varying vec3 vLightDirection;

void main()
{
    vec4 color;
    float intensity = max(0.1, dot(vNormal, vLightDirection));

    if (uEnableTexture) {
        color = texture2D(uSampler, vTexcoord);
    } else {
        color = vec4(0.2, 1.0, 0.2, 1.0);
    }

    gl_FragColor = color * intensity;
}