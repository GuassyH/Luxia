#version 460 core

uniform bool hasDiffuse;
uniform bool hasSpecular;
uniform bool hasNormals;

uniform sampler2D diffuse0;
uniform sampler2D specular0;
uniform sampler2D normals0;

in vec3 crntPos;
in vec3 vertCol;
in vec2 texCoords;

out vec4 fragCol;
void main(){
	// fragCol = vec4(texCoords.x, texCoords.y, 1.0, 1.0);
	fragCol = vec4(1.0);
	if(hasDiffuse) fragCol *= texture(diffuse0, texCoords);
}