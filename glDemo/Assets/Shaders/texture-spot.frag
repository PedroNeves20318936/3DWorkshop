#version 450 core

// Diffuse texture - directional light

// Texture sampler (for diffuse surface colour)
layout(binding = 0) uniform sampler2D texture;

// Directional light model
uniform vec3 SPOPos;
uniform vec3 SPODir;
uniform vec3 SPOCol;
uniform vec3 SPOAmb;
uniform float SPOCutoff;
uniform float SPOOuterCutoff;


in SimplePacket {
	
	vec3 surfaceWorldPos;
	vec3 surfaceNormal;
	vec2 texCoord;

} inputFragment;


layout (location=0) out vec4 fragColour;

void main(void) {
	vec3 N = normalize(inputFragment.surfaceNormal);
	vec3 lightDir = normalize(SPOPos - inputFragment.surfaceWorldPos);

	float l = 0.0;
	vec3 spotDir = normalize(-SPODir);
	float theta = dot(lightDir, spotDir);

	float epsilon = SPOCutoff - SPOOuterCutoff;
    float intensity = clamp((theta - SPOOuterCutoff) / epsilon, 0.0, 1.0);

    l = max(dot(N, lightDir), 0.0);
    l *= intensity;

	vec4 surfaceColour = texture2D(texture, inputFragment.texCoord);
	vec3 diffuseColour = surfaceColour.rgb * SPOCol * l;

	fragColour = vec4(SPOAmb, 1.0) + vec4(diffuseColour, 1.0);
}