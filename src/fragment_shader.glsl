varying vec3 vNormal;
varying vec3 vLightDirection;

void main()
{
    vec4 color = vec4(1.0, 1.0, 1.0, 1.0);
    float intensity = max(0.1, dot(vNormal, vLightDirection));

    gl_FragColor = color * intensity;
}