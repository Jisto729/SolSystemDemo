#version 450

in vs_output_interface
{
    vec3 fragPos;
    vec3 color;
    vec3 normal;
} attribs;

out vec4 fragColor;

vec3 CalculateLight();

uniform vec3 lightPositions[];
uniform vec3 lightColors[];
uniform vec3 camPosition;
uniform int lightCount;

void main()
{
    fragColor = vec4(CalculateLight() * attribs.color,1.0f);
}

vec3 CalculateLight() 
{
    vec3 resultColor = vec3(0.0, 0.0, 0.0);
    // //TODO add all the coefficients from the light objects
    float ambientLight = 0.1;
    float diffuseLight = 0.8;
    float specularLight = 0.5;
    float specShine = 8;
    //max 5 light objects
    for(int i = 0; i < lightCount && i < 5; i++) {
        vec3 currentColor;
        vec3 currentLightPosition;
        //dynamic indexing is not available
        if(i == 0) {
            currentColor = vec3(lightColors[0][0], lightColors[0][1], lightColors[0][2]);
            currentLightPosition = vec3(lightPositions[0][0], lightPositions[0][1], lightPositions[0][2]);
        }
        if(i == 1) {
            currentColor = vec3(lightColors[1][0], lightColors[1][1], lightColors[1][2]);
            currentLightPosition = vec3(lightPositions[1][0], lightPositions[1][1], lightPositions[1][2]);
        }
        if(i == 2) {
            currentColor = vec3(lightColors[2][0], lightColors[2][1], lightColors[2][2]);
            currentLightPosition = vec3(lightPositions[2][0], lightPositions[2][1], lightPositions[2][2]);
        }
        if(i == 3) {
            currentColor = vec3(lightColors[3][0], lightColors[3][1], lightColors[3][2]);
            currentLightPosition = vec3(lightPositions[3][0], lightPositions[3][1], lightPositions[3][2]);
        }
        if(i == 4) {
            currentColor = vec3(lightColors[4][0], lightColors[4][1], lightColors[4][2]);
            currentLightPosition = vec3(lightPositions[4][0], lightPositions[4][1], lightPositions[4][2]);
        }
        //from https://learnopengl.com/Lighting/Multiple-lights
        vec3 viewDir = normalize(camPosition - attribs.fragPos);
        vec3 lightDir = normalize(currentLightPosition - attribs.fragPos);
        // diffuse shading
        float diff = max(dot(attribs.normal, lightDir), 0.0);
        // specular shading
        vec3 reflectDir = reflect(-lightDir, attribs.normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), specShine);
        // combine results
        vec3 ambient  = ambientLight * currentColor;
        vec3 diffuse  = diffuseLight * diff * currentColor;
        vec3 specular = specularLight * spec * currentColor;

        resultColor += ambient + diffuse + specular;
        }
        return resultColor;
}