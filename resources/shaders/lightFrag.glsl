#version 450

out vec4 fragColor;

uniform samplerCube lightTexture;

in vs_output_interface
{
    vec3 color;
    vec3 texCoord;
} attribs;

void main()
{
    fragColor = texture(lightTexture, attribs.texCoord);
}