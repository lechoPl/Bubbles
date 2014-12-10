#version 330 core

// Interpolated values from the vertex shaders
in vec3 Position_modelspace;
in vec3 Normal_modelspace;

// Ouput data
out vec3 color;

uniform mat4 model;
uniform vec3 cameraPosition;
uniform vec3 playerPosition;

uniform float maxDistance; // to fog effect

uniform struct Material {
	vec3 color;
	float shininess;
	bool isLightSource;
	float ambientReflection;
	float diffuseReflection;
	float specularReflection;
} material;

#define MAX_LIGHTS 20
uniform int numLights;
uniform struct Light {
	vec4 position;
	vec3 intensities; //color
	float power;
} allLights[MAX_LIGHTS];

vec3 ApplyLight(Light light, Material material, vec3 normal, vec3 surfacePos, vec3 surfaceToCamera) {
	vec3 lightDirection;
	float distance = 1.0;

	if(light.position.w == 0.0)
	{
		lightDirection = normalize(light.position.xyz);
	}
	else
	{
		lightDirection = normalize(light.position.xyz - surfacePos);
		distance = length(light.position.xyz - surfacePos);
	}

	vec3 ambient = material.color;

	vec3 diffuse = light.intensities * light.power * clamp( dot ( normalize(normal), normalize(lightDirection) ), 0, 1) / pow(distance, 2);

	vec3 speculat = light.intensities * light.power / pow(distance, 2);
	if(!material.isLightSource)
		speculat *= pow ( clamp( dot( normalize(reflect(-lightDirection, normal)), normalize(surfaceToCamera)), 0, 1), material.shininess);
		
	return material.ambientReflection * ambient +
		material.diffuseReflection * diffuse +
		material.specularReflection * speculat;
}

void main(){
	vec3 normal = normalize(transpose(inverse(mat3(model))) * Normal_modelspace);
    vec3 surfacePos = vec3(model * vec4(Position_modelspace, 1));
	vec3 surfaceToCamera = normalize(cameraPosition - surfacePos);
	
	vec3 linearColor = vec3(0);

	for(int i = 0; i < numLights; ++i)
		linearColor += ApplyLight(allLights[i], material, normal, surfacePos, surfaceToCamera);

	//fog effect
	float dist = length(playerPosition - surfacePos);
	if(dist > maxDistance)
		dist = maxDistance;
	
	float depth = 1 * dist/maxDistance;

	vec3 waterColor = vec3(0.0, 0.4f, 0.8f);
	vec3 minWaterColor = 0.5 * waterColor;
	vec3 waterScaleColor = waterColor - minWaterColor;

	float temp = pow(depth, 1);
	color = (temp) * (minWaterColor + (1.f - temp) * waterScaleColor)
			+ (1.f - temp) * linearColor;
}