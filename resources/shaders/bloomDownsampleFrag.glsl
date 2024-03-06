#version 450

//from https://learnopengl.com/Guest-Articles/2022/Phys.-Based-Bloom

uniform sampler2D texture;
uniform vec2 texRes;

uniform int mipLevel = 1;

in vs_output_interface
{
    vec2 texCoord;
} attribs;

layout (location = 0) out vec3 downsample;

void main()
{
    vec2 texelSize = 1.0 / texRes;
    float x = texelSize.x;
    float y = texelSize.y;

    // Take 13 samples around current texel:
    // a - b - c
    // - j - k -
    // d - e - f
    // - l - m -
    // g - h - i
    // === ('e' is the current texel) ===
    vec3 a = texture(texture, vec2(attribs.texCoord.x - 2*x, attribs.texCoord.y + 2*y)).rgb;
    vec3 b = texture(texture, vec2(attribs.texCoord.x,       attribs.texCoord.y + 2*y)).rgb;
    vec3 c = texture(texture, vec2(attribs.texCoord.x + 2*x, attribs.texCoord.y + 2*y)).rgb;

    vec3 d = texture(texture, vec2(attribs.texCoord.x - 2*x, attribs.texCoord.y)).rgb;
    vec3 e = texture(texture, vec2(attribs.texCoord.x,       attribs.texCoord.y)).rgb;
    vec3 f = texture(texture, vec2(attribs.texCoord.x + 2*x, attribs.texCoord.y)).rgb;

    vec3 g = texture(texture, vec2(attribs.texCoord.x - 2*x, attribs.texCoord.y - 2*y)).rgb;
    vec3 h = texture(texture, vec2(attribs.texCoord.x,       attribs.texCoord.y - 2*y)).rgb;
    vec3 i = texture(texture, vec2(attribs.texCoord.x + 2*x, attribs.texCoord.y - 2*y)).rgb;

    vec3 j = texture(texture, vec2(attribs.texCoord.x - x, attribs.texCoord.y + y)).rgb;
    vec3 k = texture(texture, vec2(attribs.texCoord.x + x, attribs.texCoord.y + y)).rgb;
    vec3 l = texture(texture, vec2(attribs.texCoord.x - x, attribs.texCoord.y - y)).rgb;
    vec3 m = texture(texture, vec2(attribs.texCoord.x + x, attribs.texCoord.y - y)).rgb;

    // Apply weighted distribution:
    // 0.5 + 0.125 + 0.125 + 0.125 + 0.125 = 1
    // a,b,d,e * 0.125
    // b,c,e,f * 0.125
    // d,e,g,h * 0.125
    // e,f,h,i * 0.125
    // j,k,l,m * 0.5
    // This shows 5 square areas that are being sampled. But some of them overlap,
    // so to have an energy preserving downsample we need to make some adjustments.
    // The weights are the distributed, so that the sum of j,k,l,m (e.g.)
    // contribute 0.5 to the final color output. The code below is written
    // to effectively yield this sum. We get:
    // 0.125*5 + 0.03125*4 + 0.0625*4 = 1
    downsample = e*0.125;
    downsample += (a+c+g+i)*0.03125;
    downsample += (b+d+f+h)*0.0625;
    downsample += (j+k+l+m)*0.125;
}
    