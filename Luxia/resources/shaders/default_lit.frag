#version 460 core

struct LightObject {
	int type; // 0 = dir, 1 = spot, 2 = point, 3 = area
	int _pad0[3];
	vec4 color;
	vec3 position;
	float pad;
	vec3 rotation; // vector
	float pad1;
};

layout (std430, binding = 0) buffer LightObjectsBuffer{
	LightObject lightObjects[];
};

uniform int numLights;

uniform bool hasDiffuse;
uniform bool hasSpecular;
uniform bool hasNormals;

uniform sampler2D diffuse0;
uniform sampler2D specular0;
uniform sampler2D normals0;

uniform vec4 mat_color;
uniform float mat_roughness;
uniform float mat_metallic;

in vec3 crntPos;
in vec3 vertCol;
in vec2 texCoords;
in vec3 vertNormal;


vec3 CalculateNormal(){
	vec3 result = vertNormal;
	
	return result;
}

out vec4 fragCol;
void main(){
	fragCol = mat_color;
	vec3 normal = vertNormal;
	vec3 lightCol = vec3(1);


	if(hasDiffuse) fragCol *= texture(diffuse0, texCoords);
	if(hasNormals) normal = CalculateNormal();
	
	for (int i = 0; i < numLights; i++){
		LightObject lo = lightObjects[i];

		if(lo.type == 0){
			lightCol *= lo.color.rgb * (dot(normalize(vertNormal), normalize(lo.rotation)) + 1) / 2;
		}
	}

	fragCol *= vec4(lightCol, 1.0);
}