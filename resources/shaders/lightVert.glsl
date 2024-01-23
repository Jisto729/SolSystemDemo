#version 450

in layout(location = 0) vec3 position;
in layout(location = 1) vec3 color;
in layout(location = 2) float modelId;
in layout(location = 3) vec3 texCoord;

layout(std430, binding=0) buffer perDrawData_t
{
    mat4 model[];
};

out vs_output_interface
{
    vec3 color;
    vec3 texCoord;
} attribs;

uniform mat4 camMatrix;

void main()
{
    attribs.color = color;
    attribs.texCoord = position;
    int modelIndex = int(modelId);
    gl_Position = camMatrix * model[modelIndex] * vec4(position,1.0f);
}