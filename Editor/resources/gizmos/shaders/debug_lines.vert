#version 460 core


uniform mat4 projMat;
uniform mat4 viewMat;

out vec3 crntPos;

void main(){
	
	gl_Position = projMat * viewMat * vec4(crntPos, 1.0);
}