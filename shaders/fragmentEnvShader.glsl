#version 330 core

in vec3 Normal;
in vec3 Position;
in vec3 FragPos;

out vec4 color;

uniform vec3 viewPos;
uniform samplerCube cubemap;

uniform bool fogOn;
uniform float fogIntensity;
uniform float fogBrightness;

float CalculateFog() 
{
    float dist = distance(FragPos, viewPos);
    float res = fogOn ? exp(-fogIntensity * dist) : 1.0;
    return 1.0 - clamp(res, 0.0, 1.0);
}


void main()
{
    vec3 I = normalize(Position - viewPos);
    vec3 R = reflect(I, normalize(Normal));
    vec4 outputColor = texture(cubemap, R); 

    color = mix(outputColor, vec4(fogBrightness, fogBrightness, fogBrightness, 1.0f), CalculateFog());
}