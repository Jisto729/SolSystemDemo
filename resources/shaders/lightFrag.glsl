#version 450

out vec4 fragColor;

in vs_output_interface
{
    vec3 color;
} attribs;

void main()
{
	fragColor = vec4(attribs.color, 1.0);
}