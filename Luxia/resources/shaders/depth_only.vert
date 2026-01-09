#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTex;

uniform mat4 modelMat;
uniform mat4 projMat;
uniform mat4 viewMat;


void main(){
	vec3 crntPos = vec3(modelMat * vec4(aPos, 1.0));

	gl_Position = projMat * viewMat * vec4(crntPos, 1.0);
}