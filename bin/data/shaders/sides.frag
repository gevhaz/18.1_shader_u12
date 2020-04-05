#version 450

uniform int glowLength;
uniform int width;
uniform int height;
uniform float dampening;

out vec4 outputColor;

in vec2 newTexcoord;

void main() {

	float dist = newTexcoord.x;
	float intensity = newTexcoord.y;

	intensity *= dampening;

	float glow = 0.5 * exp((-pow((dist - width / 2), 2) - 60) / (
				(1.0 + intensity) * 18.0));
	
	vec3 white = vec3(1.0);
	
	outputColor = vec4(white, glow);

}
