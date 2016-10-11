#version 400

in vec3 vertexPosition;
in vec3 vertexColor;
uniform mat4 projMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;

out vec3 vertexColorVF;

void main(){
	vertexColorVF=vertexColor;
	gl_Position = projMatrix*viewMatrix* modelMatrix * vec4(vertexPosition,1);
}