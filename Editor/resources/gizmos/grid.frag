#version 460 core


in vec3 crntPos;
in vec3 vertCol;
in vec2 texCoords;
uniform vec3 camPos;

out vec4 fragCol;
void main(){
	fragCol = vec4(1.0);
	float dist = distance(vec3(camPos.x, 0, camPos.z), vec3(crntPos.x, 0, crntPos.z)) / 50.0;

	if (dist > 1.0) { discard; }

	float x_pos = ((texCoords.x*1000.0) + camPos.x) * 2.0;
	float z_pos = ((texCoords.y*1000.0) + camPos.z) * 2.0;

	float gx = mod(round(x_pos * 50.0) / 50.0, 2.0);
	float gz = mod(round(z_pos * 50.0) / 50.0, 2.0);

	// SHOULD ANTI ALIAS (but i dont know how)
	float dst_mul = dist / 1000;
	bool is_grid_line = (abs(gx) < 0.05) || (abs(gz) < 0.05);

	if(is_grid_line){
		fragCol = vec4(0.9) * (1 - dist);
	}
	else{
		discard;
	}
}