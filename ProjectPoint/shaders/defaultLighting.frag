#version 460 core

#define MAX_POINT_LIGHTS 32
#define MAX_SPOT_LIGHTS 32

out vec4 FragColor;

in VS_OUT
{
	vec2 TexCoord;
	vec3 Normal;
	vec3 FragPos;
	vec3 camPos;
} vs_out;

struct DirLight
{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight
{
	vec3 position;
	float constant;
	float linear;
	float quadratic;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct SpotLight
{
	vec3 position;  
	float constant;
	float linear;
	float quadratic;
	float cutOff;
	float outerCutOff;
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

layout (std140, binding = 2) uniform DirectionalLight
{
	bool isDirLight;
	DirLight dirLight;
};

layout (std140, binding = 3) uniform PointLights
{
	int activePointLights;
	PointLight pointLights[MAX_POINT_LIGHTS];
};


layout (std140, binding = 4) uniform SpotLights
{
	int activeSpotLights;
	SpotLight spotLights[MAX_POINT_LIGHTS];
};


struct Material
{
	float specularStrength;
	sampler2D texture_diffuse1;
	sampler2D texture_specular1;  
};

uniform Material material;
uniform samplerCube cubeMap;

vec4 calcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec4 cubemapTexture);
vec4 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec4 cubemapTexture);  
vec4 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec4 cubemapTexture);

void main()
{
	vec3 norm = normalize(vs_out.Normal);
	vec3 viewDir = normalize(vs_out.camPos - vs_out.FragPos);
	vec4 result = vec4(0.0);

	vec3 I = normalize(vs_out.FragPos - vs_out.camPos);
    vec3 R = reflect(I, norm);
	vec4 cubemapTexture = texture(cubeMap, R);
	if(isDirLight)
	{
		result = calcDirLight(dirLight, norm, viewDir, cubemapTexture);
	}

	for(int i = 0; i < activePointLights; i++)
	{
		result += calcPointLight(pointLights[i], norm, vs_out.FragPos, viewDir, cubemapTexture);
	}

	for(int i = 0; i < activeSpotLights; i++) 
	{
		result += calcSpotLight(spotLights[i], norm, vs_out.FragPos, viewDir, cubemapTexture);
	}

	if(result.a < 0.1)
	{
		discard;
	}

	FragColor = result;
}

vec4 calcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec4 cubemapTexture)
{
	vec3 lightDir = normalize(-light.direction);

	float diff = max(dot(normal, lightDir), 0.0);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.specularStrength);

	vec4 ambient  = vec4(light.ambient, 1.0) * texture(material.texture_diffuse1, vs_out.TexCoord);
	vec4 diffuse  = vec4(light.diffuse, 1.0) * diff * texture(material.texture_diffuse1, vs_out.TexCoord);
	//vec4 diffuse  = vec4(light.diffuse, 1.0) * diff * mix(texture(material.texture_diffuse1, vs_out.TexCoord), cubeMapTexture, 0.5);
	vec4 specular = vec4(light.specular, 1.0) * spec * texture(material.texture_specular1, vs_out.TexCoord);
	return (ambient + diffuse + specular);
} 

vec4 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec4 cubemapTexture)
{
	vec3 lightDir = normalize(light.position - fragPos);

	float diff = max(dot(normal, lightDir), 0.0);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.specularStrength);

	float distance    = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

	vec4 ambient  = vec4(light.ambient, 1.0) * texture(material.texture_diffuse1, vs_out.TexCoord);
	vec4 diffuse  = vec4(light.diffuse, 1.0) * diff * texture(material.texture_diffuse1, vs_out.TexCoord);
	//vec4 diffuse  = vec4(light.diffuse, 1.0) * diff * mix(texture(material.texture_diffuse1, vs_out.TexCoord), cubeMapTexture, 0.5);
	vec4 specular = vec4(light.specular, 1.0) * spec * texture(material.texture_specular1, vs_out.TexCoord);
	ambient  *= attenuation;
	diffuse  *= attenuation;
	specular *= attenuation;
	return (ambient + diffuse + specular);
}

vec4 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec4 cubemapTexture)
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
	vec4 ambient  = vec4(light.ambient, 1.0) * texture(material.texture_diffuse1, vs_out.TexCoord);
	vec4 diffuse  = vec4(light.diffuse, 1.0) * diff * texture(material.texture_diffuse1, vs_out.TexCoord);
	//vec4 diffuse  = vec4(light.diffuse, 1.0) * diff * mix(texture(material.texture_diffuse1, vs_out.TexCoord), cubeMapTexture, 0.5);
	vec4 specular = vec4(light.specular, 1.0) * spec * texture(material.texture_specular1, vs_out.TexCoord);
	ambient *= attenuation * intensity;
	diffuse *= attenuation * intensity;
	specular *= attenuation * intensity;
	return (ambient + diffuse + specular);
}