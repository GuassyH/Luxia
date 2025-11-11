#version 460 core

uniform sampler2D cam_texture;

in vec2 texCoords;

out vec4 fragCol;
void main(){
	fragCol = texture(cam_texture, texCoords);
}