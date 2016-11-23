# version 400

const int N=3;
struct Light{
	vec3 lightColor;
	vec3 lightPosition;
	float exponent;
};


in vec3 worldPosition;
in vec3 worldNormal;

uniform vec3 cameraPosition;
uniform vec3 ambientLight;


uniform vec3 materialA;
uniform vec3 materialD;
uniform vec3 materialS;

//uniform vec3 specularLight;

uniform Light lights[N];

out vec4 pixelColor;

void main(){
	vec3 n = normalize(worldNormal);
	vec3 tempPixelColor=vec3(0,0,0);
	vec3 l;
	float factorD;
	vec3 v=normalize(cameraPosition - worldPosition);;
	vec3 r;
	float factorS;
	
	int i;
	for(i=0;i<N;i++){
		l=normalize(lights[i].lightPosition - worldPosition);
		factorD=clamp(dot(l,n),0,1);
		r =normalize(((2*n)*dot(n,l))-l);
		factorS= clamp(pow(dot(v,r),lights[i].exponent),0,1);
		tempPixelColor+=(ambientLight* materialA)+lights[i].lightColor*((materialD*factorD)+(materialS*factorS));
	}
	 
	
	
	pixelColor = vec4(clamp(tempPixelColor,0,1),1);
	

}