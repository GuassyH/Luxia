#version 460 core

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

out vec4 fragCol;
void main(){
	fragCol = mat_color;

	if(hasDiffuse) fragCol *= texture(diffuse0, texCoords);
	fragCol *= (dot(vertNormal, vec3(0, 0, 1)) + 1) / 2;
}