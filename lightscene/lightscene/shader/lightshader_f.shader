#version 330 core
out vec4 FragColor;
uniform vec4 u_light_color;
void main() {
	FragColor = u_light_color;
}