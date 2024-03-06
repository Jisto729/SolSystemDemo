#version 450

in layout(location = 0) vec2 position;
in layout(location = 1) vec2 texCoord;

out vs_output_interface
{
    vec2 texCoord;
} attribs;

void main()
{
	gl_Position = vec4(position.x, position.y, 0.0, 1.0);
	attribs.texCoord = texCoord;
}