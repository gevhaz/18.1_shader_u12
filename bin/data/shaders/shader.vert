#version 450

uniform mat4 modelViewProjectionMatrix;
uniform float angleDiff;
uniform int radius;

in vec4 position;
in vec2 texcoord;

out vec2 newTexcoord;

void main() {
	vec4 pos = position;

	int imgLength = 1000;
	int imgWidth = 1;

	int rectIndex = gl_VertexID / 4;

	float angle = angleDiff * (gl_VertexID / 4);

	pos.y -= 500;
	pos.x += radius;
	mat2 rot = mat2(cos(angle), sin(angle), -sin(angle), cos(angle));
	pos.xy = rot * pos.xy;

	newTexcoord = texcoord;

	gl_Position = modelViewProjectionMatrix * pos;
}
