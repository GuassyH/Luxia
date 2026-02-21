#version 460 core

struct LinePosition{
	vec3 start;
	float _pad0;
	vec3 stop;
	float _pad1;
};

layout (std430, binding = 0) buffer LinePositions {
	LinePosition[] linePositions;
};


void main(){

}