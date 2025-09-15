#version 150

in vec3 tposition; // name matches string in shader pgm
in vec2 texCoord;  // name matches string in shader pgm, not used here
in vec3 normal;

uniform mat4 transform; // model matrix
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;

out vec3 Normal;
out vec3 FragPos;

void main(){
	gl_Position = projection * view * transform * vec4(tposition, 1.0f);
	//Normal = normal;
	//Normal = vec3(transform * vec4(normal, 0.0f)); // w to 0 means we don't applay translate to the normal vector
	//Normal = mat3(transpose(inverse(transform))) * normal; // not available in GLSL 140... it is slow anyway...
	Normal = normalMatrix * normal;
	FragPos = vec3(transform * vec4(tposition, 1.0f));
}
