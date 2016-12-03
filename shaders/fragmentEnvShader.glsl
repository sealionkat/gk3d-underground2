#version 330 core

in vec3 Normal;
in vec3 Position;

out vec4 color;

uniform vec3 viewPos;
uniform samplerCube skybox;


void main()
{
    vec3 I = normalize(Position - viewPos);
    vec3 R = reflect(I, normalize(Normal));
    color = texture(skybox, R);
}