#version 330 core

layout(location = 0) in vec3 position;

uniform mat4 u_MVP;
uniform vec4 u_Colors;

out vec4 v_Color;

void main(){
	gl_Position = u_MVP * vec4(position.x, position.y, position.z, 1.0f);
	vec4 colors = u_Colors;
	v_Color = colors;

}