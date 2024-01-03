#version 430 core

layout( location = 0 ) in vec2 vs_in_uv;

out vec3 vs_out_pos;
out vec3 vs_out_norm;
out vec2 vs_out_tex;

uniform mat4 world;
uniform mat4 worldIT;
uniform mat4 viewProj;

uniform float elapsedTimeInSec = 0.0;

uniform float waterWidth = 0.0;
uniform float waterHight = 0.0;

vec3 GetPos(float u, float v)
{
	vec3 pos = vec3(-waterWidth/2.0, 0.0, waterHight/2.0) + vec3( waterWidth, 0.0, -waterHight) * vec3(u, 0.0, v);
	pos.y = sin( (pos.z + elapsedTimeInSec) / 8.0) + sin( (pos.y + pos.x + elapsedTimeInSec) / 6.0);
	return pos;
}

vec3 GetNorm(float u, float v)
{
	vec3 du = GetPos(u + 0.01, v) - GetPos(u - 0.01, v);
	vec3 dv = GetPos(u, v + 0.01) - GetPos(u, v - 0.01);
	return normalize(cross(du, dv));
}

void main()
{
	vec3 vs_in_pos = GetPos(vs_in_uv.x, vs_in_uv.y);
	gl_Position = viewProj * world * vec4( vs_in_pos, 1 );

	vs_out_pos = (world * vec4(vs_in_pos, 1)).xyz;
	vs_out_norm = (worldIT * vec4(GetNorm(vs_in_uv.x, vs_in_uv.y), 0)).xyz;
	vs_out_tex = vs_in_uv;
}