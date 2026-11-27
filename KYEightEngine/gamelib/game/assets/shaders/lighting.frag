#version 330

in vec2 fragTexCoord;
in vec3 fragPosition;
in vec3 fragNormal;
in vec4 fragColor;

// Rl material uniforms
uniform sampler2D texture0;
uniform vec4 colDiffuse;

// Directional light properties
uniform vec3 lightDirection;
uniform vec3 lightColour;
uniform float lightIntensity;
uniform float ambientStrength;

out vec4 finalColor;

void main() 
{
	vec3 normal = normalize(fragNormal);
	vec3 direction = normalize(lightDirection);

	float diffuse = max(dot(normal, direction), 0.0);
	
	vec3 ambient = vec3(ambientStrength);
	vec3 directional = lightColour * diffuse * lightIntensity;

	vec3 lighting = ambient + directional;

	// Apply lighting to material
	vec4 albedo = texture(texture0, fragTexCoord) * colDiffuse * fragColor;

	finalColor = vec4(albedo.rgb * lighting, albedo.a);
}