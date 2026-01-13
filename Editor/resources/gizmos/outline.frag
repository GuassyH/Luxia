#version 460 core

uniform sampler2D baseSample;
uniform sampler2D depthSample;

uniform vec2 texelSize;
uniform float nearPlane;
uniform float farPlane;
uniform float depthThreshold;

in vec2 texcoords;

float LinearizeDepth(float d){
	float z = 2.0 * d - 1.0;
	return ((2.0 * nearPlane * farPlane) / (farPlane + nearPlane - z * (farPlane - nearPlane)));
}

out vec4 fragCol;
void main(){
	vec4 base = texture(baseSample, texcoords);
	float centreDepth = texture(depthSample, texcoords).r;

	// fragCol = vec4(centreDepth);

	if (centreDepth == 1.0){
		fragCol = base;
		return;
	}

	centreDepth = LinearizeDepth(centreDepth);

	vec2 offsets[4] = vec2[](vec2(texelSize.x, 0.0), vec2(-texelSize.x, 0.0), vec2(0.0, texelSize.y), vec2(0.0, -texelSize.y));

	float edge = 0.0;
	for (int i = 0; i < 4; i++){
		float d = texture(depthSample, texcoords + offsets[i]).r;
		d = LinearizeDepth(d);
		edge = max(edge, abs(centreDepth - d));
	}

	if(edge > depthThreshold)
		fragCol = vec4(1.0, 0.55, 0.1, 1.0);
	else
		fragCol = base;
}