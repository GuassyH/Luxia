#version 460 core

uniform sampler2D cam_texture;
uniform vec2 window_size;

in vec2 texCoords;


vec4 Fit(){
	vec2 tex_raw_size = textureSize(cam_texture, 0);
	float cam_aspect = tex_raw_size.x / tex_raw_size.y;
	float window_aspect = window_size.x / window_size.y;

	vec2 tex_size;

	if (window_aspect > cam_aspect) {
		// Window is wider than camera -> match height
		tex_size.y = window_size.y;
		tex_size.x = window_size.y * cam_aspect;
	}
	else {
		// Window is taller than camera -> match width
		tex_size.x = window_size.x;
		tex_size.y = window_size.x / cam_aspect;
	}

	vec2 tex_pos;
	tex_pos.x = (window_size.x - tex_size.x) * 0.5;
	tex_pos.y = (window_size.y - tex_size.y) * 0.5;

	vec2 coord_in_screen = vec2(window_size.x * texCoords.x, window_size.y * texCoords.y);
	vec2 trans_coord = vec2(coord_in_screen.x - tex_pos.x, coord_in_screen.y - tex_pos.y);

	if(trans_coord.x >= 0.0 && trans_coord.x <= tex_size.x && trans_coord.y >= 0.0 && trans_coord.y <= tex_size.y){
		vec2 calc_coord = vec2(trans_coord.x / tex_size.x, trans_coord.y / tex_size.y);
		return texture(cam_texture, calc_coord);
	}else{
		return vec4(0.0);
	}
}

vec4 Fill(){
	vec2 tex_raw_size = textureSize(cam_texture, 0);
	float cam_aspect = tex_raw_size.x / tex_raw_size.y;
	float window_aspect = window_size.x / window_size.y;

	vec2 tex_size;

	if (window_aspect < cam_aspect) {
		// Window is wider than camera -> match height
		tex_size.y = window_size.y;
		tex_size.x = window_size.y * cam_aspect;
	}
	else {
		// Window is taller than camera -> match width
		tex_size.x = window_size.x;
		tex_size.y = window_size.x / cam_aspect;
	}

	vec2 tex_pos;
	tex_pos.x = (window_size.x - tex_size.x) * 0.5;
	tex_pos.y = (window_size.y - tex_size.y) * 0.5;

	vec2 coord_in_screen = vec2(window_size.x * texCoords.x, window_size.y * texCoords.y);
	vec2 trans_coord = vec2(coord_in_screen.x - tex_pos.x, coord_in_screen.y - tex_pos.y);

	if(trans_coord.x >= 0.0 && trans_coord.x < tex_size.x && trans_coord.y >= 0.0 && trans_coord.y < tex_size.y){
		vec2 calc_coord = vec2(trans_coord.x / tex_size.x, trans_coord.y / tex_size.y);
		return texture(cam_texture, calc_coord);
	}else{
		return vec4(0.0);
	}
}

vec4 Stretch(){
	return texture(cam_texture, texCoords);
}

out vec4 fragCol;
void main(){
	fragCol = Fill();
}