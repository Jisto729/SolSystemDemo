#version 450

// layout (location = 0) out vec4 fragColor;
// layout (location = 1) out vec4 BrightColor; 

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