#version 400

in vec3 vertexToGS[];
in vec3 normalToGS[];
in vec3 colorToGS[];

uniform mat4 projectionMatrix, modelMatrix, viewMatrix;
uniform bool shot;

out vec3 colorToFS;

layout(triangles, invocations = 1) in;
layout(triangle_strip, max_vertices = 6) out;

void main() {
	mat4 matrix = projectionMatrix * viewMatrix * modelMatrix;
	if(!shot) { 
		for(int i = 0; i < vertexToGS.length(); i++) {
			gl_Position = matrix * vec4(vertexToGS[i], 1);
			colorToFS = colorToGS[i];
			EmitVertex();
		} 
	} else {
		vec3 position;
		for(int i = 0; i < vertexToGS.length(); i++) {
			position = vertexToGS[i] * 0.8;
			position += normalToGS[i] * 0.2;
			gl_Position = matrix * vec4(position, 1);
			colorToFS = colorToGS[i] / 2;
			EmitVertex();
		}
		EndPrimitive();
		for(int i = 0; i < vertexToGS.length(); i++) {
			position = vertexToGS[i] * 0.4;
			position += normalToGS[i] * 0.6;
			gl_Position = matrix * vec4(position, 1);
			colorToFS = colorToGS[i] * 0.65;
			EmitVertex();
		} 
	}
	EndPrimitive();
}