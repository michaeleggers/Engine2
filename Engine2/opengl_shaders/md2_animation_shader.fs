#version 150

uniform sampler2D diffuseSampler;
uniform sampler2D specularSampler;

struct Material {
    float shininess;
}; 

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

//uniform vec3 objectColor; // k
uniform vec3 lightColor;  // I

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

in vec3 FragPos;
in vec3 Normal;
in vec2 textureCoordinates;

out vec4 color;



void main()
{
	/* ambient */
	//float ambientStrength = 1.0f; 
	vec3 ambient = light.ambient * vec3(texture(diffuseSampler, textureCoordinates)); // Ia

	/* diffuse */
	vec3 norm = normalize(Normal); // vertices normal vector normalized
	vec3 lightDir = normalize(light.position - FragPos); // vector from frag to light
	float diff = max(dot(norm, lightDir), 0.0); 
	vec3 diffuse = light.diffuse * diff * vec3(texture(diffuseSampler, textureCoordinates));

	/* specular */
	//float specularStrength = 1.0f;
	vec3 viewDirection = normalize(viewPos - FragPos); // vector from frag to camera
	vec3 lightReflection = reflect(-lightDir, norm); // reflect function expects vector from light to frag
	float spec = pow( max(dot(viewDirection, lightReflection), 0.0), material.shininess ); // last argument is shinyness
	vec3 specular =  light.specular * (spec * vec3(texture(specularSampler, textureCoordinates))); 

	/* final fragment color */
	vec3 result = ambient + diffuse + specular;
    color = vec4(result, 1.0f);
}

