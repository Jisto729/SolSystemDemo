// // #version 330
// #version 450

// in layout(location = 0) vec3 position;
// in layout(location = 1) vec3 color;

// layout(std430, binding=0) buffer perDrawData_t
// {
//     mat4 model[];
// };

// out vec3 currPos;

// uniform mat4 camMatrix;

// out vs_output_interface
// {
//     vec3 color;
// }attribs;

// void main()
// {
//     attribs.color = color;
//     // = vec3(model[0] * vec4(aPos, 1.0f));
//     // gl_Position = camMatrix * model[0] *  vec4(position,1.0f);
//     // gl_Position = camMatrix * vec4(currPos, 1.0);
//     gl_Position = vec4(position,1.0f);
// }

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


//uniform mat4 model;
uniform mat4 camMatrix;

void main()
{
    attribs.color = color;
    gl_Position = camMatrix * model[0] *  vec4(position,1.0f);
}