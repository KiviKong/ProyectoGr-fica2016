#version 400

in vec3 vertexPosition;
in vec3 vertexNormal;

uniform mat4 projMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;



out vec3 worldPosition;
out vec3 worldNormal;

void main(){
	vec4 world = modelMatrix*vec4(vertexPosition,1);
	worldPosition=world.xyz;
	gl_Position = projMatrix*viewMatrix* world;
	
	mat4 G = transpose(inverse(modelMatrix));
	vec4 n4 = G*vec4(vertexNormal,0);
	worldNormal = n4.xyz;
	
}