#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoords;
layout(location = 2) in vec4 colors;
layout(location = 3) in vec3 normals;

uniform mat4 u_VP;
uniform mat4 u_Model;

out vec4 v_Color;
out vec2 v_TexCoords;
out vec3 v_Normal;
out vec3 v_FragPos;


void main(){
	mat4 MVP = u_VP * u_Model;
	gl_Position = MVP * vec4(position.x, position.y, position.z, 1.0f);
	v_Color = colors;
	v_Normal = mat3(transpose(inverse(u_Model))) * normals;
	v_TexCoords = texCoords;
	v_FragPos = vec3(u_Model * vec4(position, 1.0f));
}


#shader fragment
#version 330 core


in vec4 v_Color;
in vec2 v_TexCoords;
in vec3 v_Normal;
in vec3 v_FragPos;

uniform vec4 u_RGBA;
uniform sampler2D u_Texture;

uniform vec3 u_LightColor;
uniform vec3 u_LightPos;
uniform vec3 u_CameraPosition;
uniform float u_DiffuseStrength;
uniform float u_AmbientStrength;
uniform float u_SpecularStrength;
uniform float u_Shininess;


out vec4 f_Color;


void main(){ 
	f_Color = vec4(0.5f, 0.5f, 0.5f, 1.0f);
	vec3 norm = normalize(v_Normal);
	
	vec3 LightDirection = normalize(u_LightPos - v_FragPos);
	float diff = max(dot(abs(norm), LightDirection), 0.0);
	vec3 DiffuseLight = diff * u_LightColor * u_DiffuseStrength;
	vec3 AmbientLight = u_LightColor * u_AmbientStrength;
	vec3 viewDir = normalize(u_CameraPosition - v_FragPos);
	vec3 reflectDir = reflect(-LightDirection, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Shininess);
	vec3 SpecularLight = u_SpecularStrength * spec * u_RGBA.xyz;
	f_Color = (vec4(DiffuseLight, 1.0f) + vec4(AmbientLight, 1.0f)) * u_RGBA;
	f_Color = f_Color + vec4(SpecularLight, 1.0f);
	vec4 texColor = texture(u_Texture, v_TexCoords);
	if (texColor.rgb == vec3(0.0f, 0.0f, 0.0f)){
		f_Color = (vec4(DiffuseLight, 1.0f) + vec4(AmbientLight, 1.0f)) * u_RGBA;
		f_Color = f_Color + vec4(SpecularLight, 1.0f);
	}
	else {
		f_Color = texColor * vec4(1.0f, 1.0f, 1.0f, 0.2);
	}
//	f_Color = vec4(norm * 255.0, 1.0f);		//Uncomment to check if normals exist
}