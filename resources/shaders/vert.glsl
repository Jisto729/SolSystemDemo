#version 450

in layout(location = 0) vec3 position;
in layout(location = 1) vec3 color;
//should probably be unsigned int, but can't seem to make that work
in layout(location = 2) float modelId;
in layout(location = 3) vec3 normal;

layout(std430, binding=0) buffer perDrawData_t
{
    mat4 model[];
};

out vs_output_interface
{
    vec3 fragPos;
    vec3 color;
    vec3 normal;
} attribs;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

void main()
{
    int modelIndex = int(modelId);
    attribs.fragPos = vec3(model[modelIndex] * vec4(position, 1.0f));
    attribs.color = color;
    attribs.normal = normalize(mat3(model[modelIndex]) * normal);
    gl_Position = projectionMatrix * viewMatrix * model[modelIndex] * vec4(position,1.0f);
}