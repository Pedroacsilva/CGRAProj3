#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoords;
layout(location = 2) in vec4 colors;
layout(location = 3) in vec3 normals;

uniform mat4 u_MVP;
uniform mat4 u_Model;
uniform vec4 u_Colors;

out vec4 v_Color;
out vec2 v_TexCoords;
out vec3 v_Normal;
out vec3 v_FragPos;

void main(){
	gl_Position = u_MVP * vec4(position.x, position.y, position.z, 1.0f);
	vec4 colors = u_Colors;
	v_Color = colors;
	v_TexCoords = texCoords;
	v_Normal = normalize(vec3(u_Model * vec4(position.xyz, 1.0f)));
	// Passar posição do vértice para extrapolação no fragment shader
	v_FragPos = vec3(u_Model * vec4(position, 1.0f));
}