#version 460 core
out vec4 fragColor;

in vec2 texCoords;
in vec3 camPos;
in vec3 fragPos;
in vec3 normal;

struct DirLight
{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight
{
    float constant;
    float linear;
    float quadratic;

    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

};

struct SpotLight
{
    float constant;
    float linear;
    float quadratic;

    float cutOff;
    float outerCutOff;

    vec3 position;  
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Material
{
    float specularStrength;
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;  
    sampler2D texture_opacity1;
};


#define NR_POINT_LIGHTS 32
#define NR_SPOT_LIGHTS 32

uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLights[NR_POINT_LIGHTS];
uniform Material material;

uniform bool isDirLight;

uniform int activePointLights;
uniform int activeSpotLights;

vec4 calcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec4 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);  
vec4 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    vec3 norm = normalize(normal);
    vec3 viewDir = normalize(camPos - fragPos);
    vec4 result = vec4(0.0);
    if(isDirLight)
    {
        result = calcDirLight(dirLight, norm, viewDir);
    }

    for(int i = 0; i < activePointLights; i++)
    {
        result += calcPointLight(pointLights[i], norm, fragPos, viewDir);
    }

    for(int i = 0; i < activeSpotLights; i++) 
    {
        result += calcSpotLight(spotLights[i], norm, fragPos, viewDir);
    }

    fragColor = vec4(result.xyz, texture(material.texture_opacity1, texCoords));
}

vec4 calcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.specularStrength);

    vec4 ambient  = vec4(light.ambient, 1.0) * texture(material.texture_diffuse1, texCoords);
    vec4 diffuse  = vec4(light.diffuse, 1.0) * diff * texture(material.texture_diffuse1, texCoords);
    vec4 specular = vec4(light.specular, 1.0) * spec * texture(material.texture_specular1, texCoords);
    return (ambient + diffuse + specular);
} 

vec4 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.specularStrength);

    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

    vec4 ambient  = vec4(light.ambient, 1.0) * texture(material.texture_diffuse1, texCoords);
    vec4 diffuse  = vec4(light.diffuse, 1.0) * diff * texture(material.texture_diffuse1, texCoords);
    vec4 specular = vec4(light.specular, 1.0) * spec * texture(material.texture_specular1, texCoords);
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec4 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{   
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.specularStrength);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec4 ambient  = vec4(light.ambient, 1.0) * texture(material.texture_diffuse1, texCoords);
    vec4 diffuse  = vec4(light.diffuse, 1.0) * diff * texture(material.texture_diffuse1, texCoords);
    vec4 specular = vec4(light.specular, 1.0) * spec * texture(material.texture_specular1, texCoords);
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}