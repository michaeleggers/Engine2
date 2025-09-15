#version 150

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;    
    float shininess;
}; 

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform vec3 objectColor; // k
uniform vec3 lightColor;  // I
uniform vec3 lightPos;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

in vec3 FragPos;
in vec3 Normal;

out vec4 color;



void main()
{
	/* ambient */
	//float ambientStrength = 1.0f; 
	vec3 ambient = material.ambient * light.ambient; // Ia

	/* diffuse */
	vec3 norm = normalize(Normal); // vertices normal vector normalized
	vec3 lightDir = normalize(light.position - FragPos); // vector from frag to light
	float diff = max(dot(norm, lightDir), 0.0); 
	vec3 diffuse = material.diffuse * (diff * light.diffuse);

	/* specular */
	//float specularStrength = 1.0f;
	vec3 viewDirection = normalize(viewPos - FragPos); // vector from frag to camera
	vec3 lightReflection = reflect(-lightDir, norm); // reflect function expects vector from light to frag
	float spec = pow( max(dot(viewDirection, lightReflection), 0.0), material.shininess ); // last argument is shinyness
	vec3 specular =  light.specular * (spec * material.specular); 

	/* final fragment color */
	vec3 result = ambient + diffuse + specular;
    color = vec4(result, 1.0f);
}
