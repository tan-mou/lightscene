#version 330 core
layout(location=0) in vec3 position;
layout(location=1) in vec3 anormal;
uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
out vec3  normal;
out vec3 object_position;

void main(){


	gl_Position= u_projection*u_view*u_model*vec4(position,1);
	object_position = vec3(u_model*vec4(position,1));
	normal = transpose(inverse(mat3(u_model)))*anormal;
}