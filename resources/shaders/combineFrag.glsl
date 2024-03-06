#version 450

out vec4 FragColor;

in vs_output_interface
{
    vec2 texCoord;
} attribs;

uniform sampler2D scene;
uniform sampler2D bloomBlur;

void main()
{                 
    float exposure = 1.0;
    float bloomStrength = 0.07;
    
    vec3 hdrColor = texture(scene, attribs.texCoord).rgb;
    vec3 bloomColor = texture(bloomBlur, attribs.texCoord).rgb;
    vec3 result = mix(hdrColor, bloomColor, bloomStrength); 

    // tone mapping
    result = vec3(1.0) - exp(-result * exposure);
    // also gamma correct while we're at it
    const float gamma = 1.6;
    result = pow(result, vec3(1.0 / gamma));

    FragColor = vec4(result, 1.0);
}