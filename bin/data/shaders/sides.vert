#version 450

uniform mat4 modelViewProjectionMatrix;
uniform float angle;
uniform float dampening;

in vec4 position;
in vec2 texcoord;

out vec2 newTexcoord;

void main() {

	vec4 pos = position;

	if (gl_VertexID / 4 == 1) {
		mat2 flipMat = mat2(-1.0, 0.0, 0.0, -1.0); //180 degrees rotation
		pos.xy = flipMat * pos.xy;
	}

	if (gl_VertexID % 4 == 0 || gl_VertexID % 4 == 1) {
		pos.y *= (0.7+dampening);
	}

	mat2 rot = mat2(cos(angle), sin(angle), -sin(angle), cos(angle));
	pos.xy = rot * pos.xy;

	newTexcoord = texcoord;

	gl_Position = modelViewProjectionMatrix * pos;

}
