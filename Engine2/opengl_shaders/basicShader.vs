#version 140

out vec2 texCoord0; // out: pass data to next render stage (fragment shader)

in vec3 tposition; // name matches string in shader pgm
in vec2 texCoord;  // name matches string in shader pgm

uniform mat4 transform; // model matrix
uniform mat4 view;
uniform mat4 projection;

void main(){
	gl_Position = projection * view * transform * vec4(tposition, 1.0);
	texCoord0 = texCoord; // pass data to the fragment shader
}