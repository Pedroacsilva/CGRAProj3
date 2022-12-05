#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoords;
layout(location = 2) in vec4 colors;

out vec4 v_Colors;

uniform mat4 u_VP;
uniform mat4 u_Model;

void main(){
	mat4 MVP = u_VP * u_Model;
	gl_Position = MVP * vec4(position.x, position.y, position.z, 1.0f);
	v_Colors = colors;
}


#shader fragment
#version 330 core
in vec4 v_Colors;


out vec4 f_Color;
void main(){
//	f_Color = vec4(1.0f, 0.3f, 0.3f, 1.0f);
	f_Color = v_Colors;
}
