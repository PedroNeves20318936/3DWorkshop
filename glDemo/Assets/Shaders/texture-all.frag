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

in SimplePacket {
	
	vec3 surfaceWorldPos;
	vec3 surfaceNormal;
	vec2 texCoord;

} inputFragment;


layout (location=0) out vec4 fragColour;

void main(void) {
	vec3 N = normalize(inputFragment.surfaceNormal);
	vec3 lightDir = normalize(POIPos - inputFragment.surfaceWorldPos);

	float lambert = max(dot(N, lightDir), 0.0);

	float distance = length(POIPos - inputFragment.surfaceWorldPos);

	float attenuation = 1.0 / (POIConstant + POILinear * distance + POIQuadratic * (distance * distance));
	attenuation = clamp(attenuation, 0.0, 1.0);

	vec4 surfaceColour = texture2D(texture, inputFragment.texCoord);
	vec3 diffuse = surfaceColour.rgb * POICol * lambert * attenuation;

	vec3 ambient = POIAmb * attenuation;

	fragColour = vec4(ambient + diffuse, 0.5f);
}
