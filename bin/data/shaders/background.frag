#version 450

uniform float alpha;

out vec4 outputColor;

void main () {
	outputColor = vec4(0.0, 0.0, 0.0, alpha);
}
