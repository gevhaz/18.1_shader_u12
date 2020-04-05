#version 450

uniform int lineLength;
uniform float intensity; //Between 0 and 1

out vec4 outputColor;

in vec2 newTexcoord;

void main() {

	float dist = newTexcoord.y;

	float glow = 1. * exp(-pow(dist - lineLength / 2, 2)  / 40.0 /
			(intensity * 500 + 1.0));

	outputColor = vec4(1.0, 1.0, 1.0, glow);
}
