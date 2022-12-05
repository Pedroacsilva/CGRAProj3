#version 330 core
in vec4 v_Color;
out vec4 f_Color;

void main(){
	f_Color = vec4(1.0f, 0.3f, 0.3f, 1.0f);
	f_Color = v_Color;
}
