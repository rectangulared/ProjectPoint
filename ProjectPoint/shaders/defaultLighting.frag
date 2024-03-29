#version 460 core

#define MAX_POINT_LIGHTS 32
#define MAX_SPOT_LIGHTS 32

out vec4 fragColor;

in VS_OUT
{
	vec2 texCoords;
	vec3 normal;
	vec3 fragPos;
	vec3 camPos;
	vec4 fragPosLightSpace;
	vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} fs_in;

struct DirLight
{
	vec3 direction;
	vec3 position;
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
	sampler2D texture_normal1;  
};

uniform Material material;
uniform sampler2D shadowMap;

uniform bool IS_SHADOW_PASS_ACTIVE;
uniform bool IS_PHONG_SHADING_ACTIVE;
uniform bool IS_NORMAL_MAPS_ACTIVE;

uniform float gamma;

vec4 calcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec4 calcPointLight(PointLight light, vec3 normal, vec3 viewDir);  
vec4 calcSpotLight(SpotLight light, vec3 normal, vec3 viewDir);
float shadowCalculation(vec4 fragPosLightSpace, vec3 lightDir);

void main()
{
	if(IS_PHONG_SHADING_ACTIVE)
	{
		vec3 norm;
		if(IS_NORMAL_MAPS_ACTIVE)
		{
			norm= texture(material.texture_normal1, fs_in.texCoords).rgb;
			norm = normalize(norm * 2.0 - 1.0);
		}
		else
		{
			norm = normalize(fs_in.normal);
		}
		vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.fragPos);
		vec4 result;

		vec3 I = normalize(fs_in.fragPos - fs_in.camPos);
		vec3 R = reflect(I, norm);
		if(isDirLight)
		{
			result = calcDirLight(dirLight, norm, viewDir);
		}

		for(int i = 0; i < activePointLights; i++)
		{
			result += calcPointLight(pointLights[i], norm, viewDir);
		}

		for(int i = 0; i < activeSpotLights; i++) 
		{
			result += calcSpotLight(spotLights[i], norm, viewDir);
		}

		vec3 mapped = vec3(1.0) - exp(-result.rgb * 1.0);
		mapped = pow(mapped, vec3(1.0 / gamma));
		fragColor = vec4(mapped, 1.0);
	} else
	{
		fragColor = texture(material.texture_diffuse1, fs_in.texCoords);
	}
}

vec4 calcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);

	vec4 ambient = vec4(light.ambient, 1.0) * texture(material.texture_diffuse1, fs_in.texCoords);
	if(ambient.a < 0.01)
	{
		discard;
		return vec4(0.0);
	}

	float diff = max(dot(normal, lightDir), 0.0);
	vec4 diffuse  = vec4(light.diffuse, 1.0) * diff * texture(material.texture_diffuse1, fs_in.texCoords);
	vec4 specular = vec4(0.0f);
	if(diffuse != vec4(0.0f))
	{
		vec3 halfwayDir = normalize(lightDir + viewDir);  
		float spec = pow(max(dot(normal, halfwayDir), 0.0), material.specularStrength);
		specular = vec4(light.specular, 1.0) * spec * texture(material.texture_specular1, fs_in.texCoords);
	}

	vec3 lighting;

	if(IS_SHADOW_PASS_ACTIVE)
	{
		float shadow = shadowCalculation(fs_in.fragPosLightSpace, lightDir);
		vec3 color = texture(material.texture_diffuse1, fs_in.texCoords).rgb;
		lighting = (ambient.rgb + (1.0 - shadow) * (diffuse.rgb + specular.rgb)) * color;
	}
	else
	{
		lighting = ambient.rgb * diffuse.rgb * specular.rgb;
	}
	return vec4(lighting, 1.0);
} 

vec4 calcPointLight(PointLight light, vec3 normal, vec3 viewDir)
{
	vec4 ambient = vec4(light.ambient, 1.0) * texture(material.texture_diffuse1, fs_in.texCoords);

	if(ambient.a < 0.01)
	{
		discard;
		return vec4(0.0);
	}

	vec3 lightDir = normalize(light.position - fs_in.fragPos);
	float diff = max(dot(normal, lightDir), 0.0);

	vec3 halfwayDir = normalize(lightDir + viewDir);  
	float spec = pow(max(dot(normal, halfwayDir), 0.0), material.specularStrength);

	float distance = length(light.position - fs_in.fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	vec4 diffuse = vec4(light.diffuse, 1.0) * diff * texture(material.texture_diffuse1, fs_in.texCoords);
	vec4 specular = vec4(0.0f);
	if(diffuse != vec4(0.0f))
	{
		specular = vec4(light.specular, 1.0) * spec * texture(material.texture_specular1, fs_in.texCoords);
	}
	ambient  *= attenuation;
	diffuse  *= attenuation;
	specular *= attenuation;
	return (ambient + diffuse + specular);
}

vec4 calcSpotLight(SpotLight light, vec3 normal, vec3 viewDir)
{   
	vec4 ambient = vec4(light.ambient, 1.0) * texture(material.texture_diffuse1, fs_in.texCoords);

	if(ambient.a < 0.01)
	{
		discard;
		return vec4(0.0);
	}

	vec3 lightDir = normalize(light.position - fs_in.fragPos);
	float diff = max(dot(normal, lightDir), 0.0);

	vec3 halfwayDir = normalize(lightDir + viewDir);  
	float spec = pow(max(dot(normal, halfwayDir), 0.0), material.specularStrength);

	float distance = length(light.position - fs_in.fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

	float theta = dot(lightDir, normalize(-light.direction)); 
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

	vec4 diffuse = vec4(light.diffuse, 1.0) * diff * texture(material.texture_diffuse1, fs_in.texCoords);
	vec4 specular = vec4(0.0f);
	if(diffuse != vec4(0.0f))
	{
		specular = vec4(light.specular, 1.0) * spec * texture(material.texture_specular1, fs_in.texCoords);
	}
	ambient *= attenuation * intensity;
	diffuse *= attenuation * intensity;
	specular *= attenuation * intensity;
	return (ambient + diffuse + specular);
}

float shadowCalculation(vec4 fragPosLightSpace, vec3 lightDir)
{	
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	if(projCoords.z > 1.0)
	{
        return 0.0;
    }
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float bias = max(0.05 * (1.0 - dot(fs_in.normal, lightDir)), 0.005);  
	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
	for(int x = -1; x <= 1; ++x)
	{
		for(int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
		}    
	}

	shadow /= 9.0;
    return shadow;
}
