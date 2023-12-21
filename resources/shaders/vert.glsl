#version 450

in layout(location = 0) vec3 position;
in layout(location = 1) vec3 color;

layout(std430, binding=0) buffer perDrawData_t
{
    mat4 model[];
};

out vs_output_interface
{
    vec3 color;
}attribs;

uniform mat4 camMatrix;
uniform int modelId;

void main()
{
    attribs.color = color;
    gl_Position = camMatrix * model[modelId] * vec4(position,1.0f);
}