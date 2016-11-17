#version 400

in vec3 vertexPosition;
in vec3 vertexColor;
in vec2 texCoord;
uniform mat4 projMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;

out vec3 vertexColorVF;
out vec2 texCoordVF;

void main(){
	vertexColorVF=vertexColor;
	texCoordVF = texCoord;
	gl_Position = projMatrix*viewMatrix* modelMatrix * vec4(vertexPosition,1);
}
