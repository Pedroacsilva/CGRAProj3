#version 330 core

in vec4 v_Color;
in vec2 v_TexCoords;
//smooth in vec2 texcords;
uniform sampler2D u_Texture;
out vec4 f_Color;

void main(){
    vec4 texColor = texture(u_Texture, v_TexCoords);
    if (texColor.rgb == vec3(0.0f, 0.0f, 0.0f)){
//        f_Color = vec4(v_TexCoords.xy, 0.0f, 1.0f);
        if (f_Color == vec4(0.0f, 0.0f, 0.0f, 1.0f)) {
            f_Color = v_Color;
        }
    }
    else
        //f_Color = min(vec4(1.0f, 1.0f, 1.0f, 1.0f), texColor * vec4(1.0f, 1.0f, 1.0f, 0.2));
        f_Color = min(vec4(1.0f, 1.0f, 1.0f, 1.0f), texColor * v_Color);

//    f_Color = vec4(v_TexCoords.xy, 0.0f, 1.0f);
        //f_Color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}