#version 400

const int N = 3;

struct Light {
	vec3 lightColor;
	vec3 lightPosition;
	float exponent;
	vec3 lightDirection;
	float cutoff;
	float subcutoff;
};

uniform LightBlock {
   Light lights[N];
};

in vec3 worldVertexPosition;
in vec3 worldVertexNormal;
in vec3 vertexColorVF;

uniform vec3 cameraPosition;
uniform vec3 ambientLight;

uniform vec3 materialA;
uniform vec3 materialD;
uniform vec3 materialS;
in vec2 vertexTextCoordVF;
uniform sampler2D mytexture;

out vec4 pixelColor;

void main() {
  vec3 n  = normalize(worldVertexNormal);
  vec3 tempPixelColor = ambientLight *  materialA;
  tempPixelColor=vec3(tempPixelColor.x*vertexColorVF.x,tempPixelColor.y*vertexColorVF.y,tempPixelColor.z*vertexColorVF.z);
  vec3 l, r;
  float factorD, factorS;
  vec3 v = normalize(cameraPosition - worldVertexPosition);

  int i;
  for(i = 0; i < N; i++) {  
  	vec3 D=normalize(lights[i].lightDirection);
	vec3 V=normalize(worldVertexPosition - lights[i].lightPosition);
	float directionFactor=0.02;
	float dist=length(worldVertexPosition - lights[i].lightPosition);
	float attenuationF=1.0/(0.01+(dist*0.05));
	float dotDV=dot(D,V);
	if(dotDV>=lights[i].cutoff){
		if(dotDV<=lights[i].subcutoff){
			directionFactor=(dotDV-lights[i].cutoff)/(lights[i].subcutoff-lights[i].cutoff);
		}else directionFactor=1.0;
	}  
  	l = normalize(lights[i].lightPosition - worldVertexPosition);  	
  	factorD = clamp(dot(l, n), 0, 1);
  	r = normalize((2 * n)  * dot(n, l) - l);
  	factorS = clamp(pow(dot(r, v), lights[i].exponent), 0, 1);
 	tempPixelColor += lights[i].lightColor * (materialD * factorD + materialS * factorS)*directionFactor*attenuationF;
  }

  pixelColor = vec4(clamp(tempPixelColor, 0, 0.4), 1);
  
}

