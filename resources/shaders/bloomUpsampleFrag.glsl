#version 450

//from https://learnopengl.com/Guest-Articles/2022/Phys.-Based-Bloom

uniform sampler2D texture;
uniform float filterRadius;

in vs_output_interface
{
    vec2 texCoord;
} attribs;

layout (location = 0) out vec3 upsample;

void main()
{
    // The filter kernel is applied with a radius, specified in texture
    // coordinates, so that the radius will vary across mip resolutions.
    float x = filterRadius;
    float y = filterRadius;

    // Take 9 samples around current texel:
    // a - b - c
    // d - e - f
    // g - h - i
    // === ('e' is the current texel) ===
    vec3 a = texture(texture, vec2(attribs.texCoord.x - x, attribs.texCoord.y + y)).rgb;
    vec3 b = texture(texture, vec2(attribs.texCoord.x,     attribs.texCoord.y + y)).rgb;
    vec3 c = texture(texture, vec2(attribs.texCoord.x + x, attribs.texCoord.y + y)).rgb;

    vec3 d = texture(texture, vec2(attribs.texCoord.x - x, attribs.texCoord.y)).rgb;
    vec3 e = texture(texture, vec2(attribs.texCoord.x,     attribs.texCoord.y)).rgb;
    vec3 f = texture(texture, vec2(attribs.texCoord.x + x, attribs.texCoord.y)).rgb;

    vec3 g = texture(texture, vec2(attribs.texCoord.x - x, attribs.texCoord.y - y)).rgb;
    vec3 h = texture(texture, vec2(attribs.texCoord.x,     attribs.texCoord.y - y)).rgb;
    vec3 i = texture(texture, vec2(attribs.texCoord.x + x, attribs.texCoord.y - y)).rgb;

    // Apply weighted distribution, by using a 3x3 tent filter:
    //  1   | 1 2 1 |
    // -- * | 2 4 2 |
    // 16   | 1 2 1 |
    upsample = e*4.0;
    upsample += (b+d+f+h)*2.0;
    upsample += (a+c+g+i);
    upsample *= 1.0 / 16.0;
}