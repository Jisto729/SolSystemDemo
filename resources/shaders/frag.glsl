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
    float ambientLight = 0.01;
    float diffuseLight = 0.8;
    float specularLight = 0.5;
    float specShine = 8;
    //max 5 light objects
    for(int i = 0; i < lightCount && i < 5; i++) {
        vec3 currentColor;
        vec3 currentLightPosition;
        //dynamic indexing is not available
        if(i == 0) {
            currentColor = lightColors[0];
            currentLightPosition = lightPositions[0];
        }
        if(i == 1) {
            currentColor = lightColors[1];
            currentLightPosition = lightPositions[1];
        }
        if(i == 2) {
            currentColor = lightColors[2];
            currentLightPosition = lightPositions[2];
        }
        if(i == 3) {
            currentColor = lightColors[3];
            currentLightPosition = lightPositions[3];
        }
        if(i == 4) {
            currentColor = lightColors[4];
            currentLightPosition = lightPositions[4];
        }
        //from https://learnopengl.com/Lighting/Multiple-lights
        vec3 viewDir = normalize(camPosition - attribs.fragPos);
        vec3 lightDir = normalize(currentLightPosition - attribs.fragPos);
        // diffuse shading
        float diff = max(dot(-attribs.normal, lightDir), 0.0);
        // specular shading
        vec3 reflectDir = reflect(-lightDir, attribs.normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), specShine);
        // combine results
        vec3 ambient  = ambientLight * currentColor;
        vec3 diffuse  = diffuseLight * diff * currentColor;
        vec3 specular = specularLight * spec * currentColor;

        resultColor += ambient + diffuse;// + specular;
        resultColor = diffuse;
        }
        return resultColor;
}