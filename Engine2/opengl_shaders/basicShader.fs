#version 150

in vec2 texCoord0; // retrieve from previous rendering stage
uniform sampler2D sampler;

void main(){
	gl_FragColor = texture2D(sampler, texCoord0); 
}
