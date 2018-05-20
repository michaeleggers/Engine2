#version 140

in vec3 tposition; // name matches string in shader pgm
in vec2 texCoord;  // name matches string in shader pgm, used again here!
in vec3 normal;
in vec3 nextVertexFrame;
in vec3 nextNormalFrame;

uniform mat4 transform; // model matrix
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;

out vec3 Normal;
out vec3 FragPos;
out vec2 textureCoordinates;

uniform float interpolation;

void main(){
	// calculate the interpolated vertex frame
	vec3 interpolatedVertex = tposition;
	if(interpolation >= 0.0f) interpolatedVertex += interpolation * (nextVertexFrame - tposition);

	//calculate the interpolated normal frame
	vec3 interpolatedNormal = normal;
	if(interpolation >= 0.0f) interpolatedNormal += interpolation * (nextNormalFrame - normal);

	gl_Position = projection * view * transform * vec4(interpolatedVertex, 1.0f);
	//Normal = normal;
	//Normal = vec3(transform * vec4(normal, 0.0f)); // w to 0 means we don't apply translate to the normal vector
	//Normal = mat3(transpose(inverse(transform))) * normal; // not available in GLSL 140... it is slow anyway...
	Normal = vec3(normalMatrix * interpolatedNormal);
	FragPos = vec3(transform * vec4(interpolatedVertex, 1.0f));
	textureCoordinates = texCoord;
}
