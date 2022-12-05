#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoords;
layout(location = 2) in vec4 colors;

uniform mat4 u_MVP;

out vec2 v_TexCoords;

void main(){
	gl_Position = u_MVP * vec4(position.x, position.y, position.z, 1.0f);
	v_TexCoords = texCoords;
}


#shader fragment
#version 330 core


in vec2 v_TexCoord;
in vec4 v_Color;
out vec4 f_Color;

uniform sampler2D u_Texture;
uniform vec4 u_Color;
void main(){
	vec4 texColor = texture(u_Texture, v_TexCoord);
	if (texColor.rgb == vec3(0.0f, 0.0f, 0.0f))
		f_Color = u_Color;
	else
		f_Color = texColor * vec4(1.0f, 1.0f, 1.0f, 0.2);

}
