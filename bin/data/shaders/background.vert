#version 450

in vec4 position;

void main() {

	vec4 pos = position;

	switch (gl_VertexID) {
		case 0: pos.xy = vec2(-1, -1); break;
		case 1: pos.xy = vec2( 1, -1); break;
		case 2: pos.xy = vec2(-1,  1); break;
		case 3: pos.xy = vec2( 1, -1); break;
		case 4: pos.xy = vec2(-1,  1); break;
		case 5: pos.xy = vec2( 1,  1); break;
	}

	gl_Position = pos;

}
