# version 400

in vec3 vertexColorVF;
in vec2 texCoordVF;

out vec4 pixelColor;

uniform sampler2D myTexture;

void main(){
	pixelColor = texture(myTexture, texCoordVF);
	// pixelColor = vec4(vertexColorVF, 1.0);
	// pixelColor = vec4(texCoordVF.x, texCoordVF.y, 0, 1.0) * texture(myTexture, texCoordVF);
}
