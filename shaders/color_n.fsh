# version 400

in vec3 vertexColorVF;
in vec2 texCoordVF;

out vec4 pixelColor;

uniform sampler2D myTexture;

void main(){
	// pixelColor = vec4(vertexColorVF, 1) * texture(myTexture, texCoordVF);
	pixelColor = vec4(vertexColorVF, 1);
}
