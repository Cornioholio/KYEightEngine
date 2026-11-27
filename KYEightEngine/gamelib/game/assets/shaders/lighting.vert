#version 330

// Vertex attributes provided
in vec3 vertexPosition;
in vec2 vertexTexCoord;
in vec3 vertexNormal;
in vec4 vertexColor;

// Standard rl uniform
uniform mat4 mvp;
uniform mat4 matModel;
uniform mat4 matNormal;

// Values passed to frag shader
out vec2 fragTexCoord;
out vec3 fragPosition;
out vec3 fragNormal;
out vec4 fragColor;

void main() 
{
	vec4 worldPosition = matModel * vec4(vertexPosition, 1.0);

	fragPosition = worldPosition.xyz;
	fragNormal = normalize((matNormal * vec4(vertexNormal, 0.0)).xyz);

	fragTexCoord = vertexTexCoord;
	fragColor = vertexColor;

	gl_Position = mvp * vec4(vertexPosition, 1.0);
}