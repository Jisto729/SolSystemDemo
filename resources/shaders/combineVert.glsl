#version 450
in layout(location = 0) vec3 position;
in layout(location = 1) vec2 texCoord;

out vs_output_interface
{
    vec2 texCoord;
} attribs;

void main()
{
    attribs.texCoord = texCoord;
    gl_Position = vec4(position, 1.0);
}