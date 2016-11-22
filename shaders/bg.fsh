# version 400

in vec3 vertexColorVF;
in vec2 texCoordVF;

out vec4 pixelColor;

uniform sampler2DRect myTexture;

void main(){
	// pixelColor = /*vec4(vertexColorVF, 0.3) */ texture(myTexture, texCoordVF);
	pixelColor = vec4(0.7, 0, 0, 1.0);
}
