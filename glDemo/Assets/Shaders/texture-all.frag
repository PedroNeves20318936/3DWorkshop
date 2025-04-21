#version 450 core

// Diffuse texture - directional light

// Texture sampler (for diffuse surface colour)
layout(binding = 0) uniform sampler2D texture;

// Directional light model
uniform vec3 POICol;
uniform vec3 POIAmb;
uniform vec3 POIPos;

uniform float POIConstant;
uniform float POILinear;
uniform float POIQuadratic;

uniform vec3 POI2Col;
uniform vec3 POI2Amb;
uniform vec3 POI2Pos;

uniform float POI2Constant;
uniform float POI2Linear;
uniform float POI2Quadratic;

uniform vec3 POI3Col;
uniform vec3 POI3Amb;
uniform vec3 POI3Pos;

uniform float POI3Constant;
uniform float POI3Linear;
uniform float POI3Quadratic;

uniform vec3 POI4Col;
uniform vec3 POI4Amb;
uniform vec3 POI4Pos;

uniform float POI4Constant;
uniform float POI4Linear;
uniform float POI4Quadratic;

uniform vec3 POI5Col;
uniform vec3 POI5Amb;
uniform vec3 POI5Pos;

uniform float POI5Constant;
uniform float POI5Linear;
uniform float POI5Quadratic;

uniform vec3 SPOPos;
uniform vec3 SPODir;
uniform vec3 SPOCol;
uniform vec3 SPOAmb;
uniform float SPOCutoff;
uniform float SPOOuterCutoff;

uniform float SPOConstant;
uniform float SPOLinear;
uniform float SPOQuadratic;

uniform vec3 DIRDir;
uniform vec3 DIRCol;
uniform vec3 DIRAmb;

in SimplePacket {
	
	vec3 surfaceWorldPos;
	vec3 surfaceNormal;
	vec2 texCoord;

} inputFragment;

layout (location=0) out vec4 fragColour;

vec3 ApplySpotLight() 
{
	vec3 N = normalize(inputFragment.surfaceNormal);
	vec3 lightDir = normalize(SPOPos - inputFragment.surfaceWorldPos);

	float l = 0.0;
	vec3 spotDir = normalize(-SPODir);
	float theta = dot(lightDir, spotDir);

	float epsilon = SPOCutoff - SPOOuterCutoff;
	float intensity = clamp((theta - SPOOuterCutoff) / epsilon, 0.0, 1.0);

	float distance = length(SPOPos - inputFragment.surfaceWorldPos);
	float attenuation = 1.0 / (SPOConstant + SPOLinear * distance + SPOQuadratic * (distance * distance));
	attenuation = clamp(attenuation, 0.0, 1.0);

	l = max(dot(N, lightDir), 0.0);
	l *= intensity * attenuation;

	vec4 surfaceColour = texture2D(texture, inputFragment.texCoord);
	vec3 diffuseColour = surfaceColour.rgb * SPOCol * l;

	return SPOAmb + diffuseColour;
}

vec3 ApplyPointLight() {
	vec3 N = normalize(inputFragment.surfaceNormal);
	vec3 lightDir = normalize(POIPos - inputFragment.surfaceWorldPos);

	float lambert = max(dot(N, lightDir), 0.0);
	float distance = length(POIPos - inputFragment.surfaceWorldPos);

	float attenuation = 1.0 / (POIConstant + POILinear * distance + POIQuadratic * (distance * distance));
	attenuation = clamp(attenuation, 0.0, 1.0);

	vec4 surfaceColour = texture2D(texture, inputFragment.texCoord);
	vec3 diffuse = surfaceColour.rgb * POICol * lambert * attenuation;

	vec3 ambient = POIAmb * attenuation;

	return ambient + diffuse;
}

vec3 ApplyPointLight2() {
	vec3 N = normalize(inputFragment.surfaceNormal);
	vec3 lightDir = normalize(POI2Pos - inputFragment.surfaceWorldPos);

	float lambert = max(dot(N, lightDir), 0.0);
	float distance = length(POI2Pos - inputFragment.surfaceWorldPos);

	float attenuation = 1.0 / (POI2Constant + POI2Linear * distance + POI2Quadratic * (distance * distance));
	attenuation = clamp(attenuation, 0.0, 1.0);

	vec4 surfaceColour = texture2D(texture, inputFragment.texCoord);
	vec3 diffuse = surfaceColour.rgb * POI2Col * lambert * attenuation;

	vec3 ambient = POI2Amb * attenuation;

	return ambient + diffuse;
}

vec3 ApplyPointLight3() {
	vec3 N = normalize(inputFragment.surfaceNormal);
	vec3 lightDir = normalize(POI3Pos - inputFragment.surfaceWorldPos);

	float lambert = max(dot(N, lightDir), 0.0);
	float distance = length(POI3Pos - inputFragment.surfaceWorldPos);

	float attenuation = 1.0 / (POI3Constant + POI3Linear * distance + POI3Quadratic * (distance * distance));
	attenuation = clamp(attenuation, 0.0, 1.0);

	vec4 surfaceColour = texture2D(texture, inputFragment.texCoord);
	vec3 diffuse = surfaceColour.rgb * POI3Col * lambert * attenuation;

	vec3 ambient = POI3Amb * attenuation;

	return ambient + diffuse;
}

vec3 ApplyPointLight4() {
	vec3 N = normalize(inputFragment.surfaceNormal);
	vec3 lightDir = normalize(POI4Pos - inputFragment.surfaceWorldPos);

	float lambert = max(dot(N, lightDir), 0.0);
	float distance = length(POI4Pos - inputFragment.surfaceWorldPos);

	float attenuation = 1.0 / (POI4Constant + POI4Linear * distance + POI4Quadratic * (distance * distance));
	attenuation = clamp(attenuation, 0.0, 1.0);

	vec4 surfaceColour = texture2D(texture, inputFragment.texCoord);
	vec3 diffuse = surfaceColour.rgb * POI4Col * lambert * attenuation;

	vec3 ambient = POI4Amb * attenuation;

	return ambient + diffuse;
}

vec3 ApplyPointLight5() {
	vec3 N = normalize(inputFragment.surfaceNormal);
	vec3 lightDir = normalize(POI5Pos - inputFragment.surfaceWorldPos);

	float lambert = max(dot(N, lightDir), 0.0);
	float distance = length(POI5Pos - inputFragment.surfaceWorldPos);

	float attenuation = 1.0 / (POI5Constant + POI5Linear * distance + POI5Quadratic * (distance * distance));
	attenuation = clamp(attenuation, 0.0, 1.0);

	vec4 surfaceColour = texture2D(texture, inputFragment.texCoord);
	vec3 diffuse = surfaceColour.rgb * POI5Col * lambert * attenuation;

	vec3 ambient = POI5Amb * attenuation;

	return ambient + diffuse;
}

vec3 ApplyDirectionalLight()
{
	vec3 N = normalize(inputFragment.surfaceNormal);
	float l = dot(N, DIRDir);

	vec4 surfaceColour = texture2D(texture, inputFragment.texCoord);
	vec3 diffuseColour = surfaceColour.rgb * DIRCol * l;

	return DIRAmb + diffuseColour;
}

void main(void) {
	vec3 lighting = ApplyDirectionalLight() + ApplyPointLight() + ApplyPointLight2() + ApplyPointLight3() + ApplyPointLight4() + ApplyPointLight5() + ApplySpotLight();
	fragColour = vec4(lighting, 1.0);
}
