#version 330 core

in vec4 v_Color;
in vec2 v_TexCoords;
in vec3 v_Normal;
in vec3 v_FragPos;


uniform sampler2D u_Texture;
uniform vec3 u_AmbientLightColor;
uniform vec3 u_LightColor;
uniform vec3 u_LightPos;
uniform vec3 u_CameraPosition;
uniform float u_AmbientStrength;
uniform float u_DirectionalDiffStrength;
uniform float u_SpecularReaction;
uniform float u_SpecularStrength;
uniform float u_Shininess;
uniform float u_WhiteLampAmbientStrength;
uniform float u_WhiteLampDiffuseStrength;
uniform float u_WhiteLampSpecularStrength;
uniform float u_ColoredLampAmbientStrength;
uniform float u_ColoredLampDiffuseStrength;
uniform float u_ColoredLampSpecularStrength;
uniform vec3 u_ColoredLampColor;
out vec4 f_Color;

void main(){
    vec3 FragColor = v_Color.rgb;
    vec3 DirectionalLightDirection = vec3(1.0f, 0.0f, 0.0f);
    vec3 WhiteLampLightDirection = normalize(vec3(3.0f, 8.5f, 0.0f) - v_FragPos);
    vec3 ColoredLampLightDirection = normalize(vec3(-3.0f, 8.5f, 0.0f) - v_FragPos);
    vec3 ViewDirection = normalize(u_CameraPosition - v_FragPos);

    float DirectionalDiffDot = max(0.0f, dot(v_Normal, DirectionalLightDirection));
    vec3 DirectionalDiffuseLight = u_AmbientLightColor * DirectionalDiffDot;
    float WhiteLampDiffDot = max(0.0f, dot(v_Normal, WhiteLampLightDirection));
    float ColoredLampDiffDot = max(0.0f, dot(v_Normal, ColoredLampLightDirection));

    // https://registry.khronos.org/OpenGL-Refpages/gl4/html/reflect.xhtml
    vec3 ReflectDirection = reflect(-DirectionalLightDirection, v_Normal);
    vec3 WhiteLightReflection = reflect(- WhiteLampLightDirection, v_Normal);
    vec3 ColoredLightReflection = reflect(- ColoredLampLightDirection, v_Normal);

    float SpecularPower = pow(dot(ViewDirection, ReflectDirection), u_Shininess);
    float WhiteLampSpecularPower = pow(dot(ViewDirection, WhiteLightReflection), u_Shininess);
    float ColoredLampSpecularPower = pow(dot(ViewDirection, ColoredLightReflection), u_Shininess);

    vec4 texColor = texture(u_Texture, v_TexCoords);
    vec3 AmbientLight = u_AmbientLightColor * u_AmbientStrength;
    vec3 WhiteAmbientLight = vec3(1.0f, 1.0f, 1.0f) * u_WhiteLampAmbientStrength;
    vec3 ColoredAmbientLight = u_ColoredLampColor * u_ColoredLampAmbientStrength;
    float WhiteDifuseDiffDot = max(0.0f, dot(v_Normal, WhiteLampLightDirection));
    float ColoredDifuseDiffDot = max(0.0f, dot(v_Normal, ColoredLampLightDirection));
    FragColor = texColor.rgb * v_Color.rgb;
    vec3 AmbientReflectedComponent =  min(vec3(1.0f), FragColor * AmbientLight);
    vec3 DirectionalDiffuseComponent = min(vec3(1.0f), FragColor * DirectionalDiffDot * u_DirectionalDiffStrength);
    vec3 SpecularLightComponent = min(vec3(1.0f),  u_AmbientLightColor * SpecularPower * u_SpecularStrength * vec3(u_SpecularReaction));

    vec3 WhiteLampAmbientComponent = min(vec3(1.0f), FragColor * u_WhiteLampAmbientStrength);
    vec3 WhiteLampDiffuseComponent = min(vec3(1.0f), FragColor * WhiteLampDiffDot * u_WhiteLampDiffuseStrength);
    vec3 WhiteLampSpecularComponent = min(vec3(1.0f), vec3(1.0f, 1.0f, 1.0f) * u_WhiteLampSpecularStrength * WhiteLampSpecularPower);

    vec3 ColoredLampAmbientComponent = min(vec3(1.0f), u_ColoredLampColor * FragColor * u_ColoredLampAmbientStrength);
    vec3 ColoredLampDiffuseComponent = min(vec3(1.0f), u_ColoredLampColor * FragColor * ColoredLampDiffDot * u_ColoredLampDiffuseStrength);
    vec3 ColoredLampSpecularComponent = min(vec3(1.0f), vec3(1.0f) * u_ColoredLampSpecularStrength * ColoredLampSpecularPower);

    vec3 DayColorSum = AmbientReflectedComponent + DirectionalDiffuseComponent + SpecularLightComponent;
    vec3 WhiteLampSum = WhiteAmbientLight + WhiteLampDiffuseComponent + WhiteLampSpecularComponent;
    vec3 ColoredLampSum = ColoredAmbientLight + ColoredLampDiffuseComponent + ColoredLampSpecularComponent;

    vec3 ColorSum = DayColorSum + WhiteLampSum + ColoredLampSum;
    f_Color = min(vec4(1.0f), vec4(ColorSum.rgb, 1.0f));
//        f_Color = vec4(WhiteLampDistance, WhiteLampDistance, WhiteLampDistance, 1.0f);
//        f_Color.rgb = SpecularLightComponent;
//        f_Color = vec4(AmbientLight.rgb, 1.0f);
//        f_Color = vec4(DirectionalDiffuseComponent.rgb, 1.0f);
//        f_Color = vec4(u_SpecularReaction, u_SpecularReaction, u_SpecularReaction, 1.0f);
//        f_Color = vec4(u_SpecularStrength, u_SpecularStrength, u_SpecularStrength, 1.0f);
//        f_Color = vec4(v_FragPos.rgb, 1.0f);
//        f_Color = vec4(ColoredLampSum.rgb, 1.0f);
//        f_Color =vec4(WhiteLampSpecularComponent.rgb, 1.0f);
//        f_Color =vec4(WhiteLampSpecularComponent.rgb + ColoredLampSpecularComponent.rgb, 1.0f);
//        f_Color = vec4(DayColorSum.rgb, 1.0f);
//    f_Color.rgb = SpecularLightComponent;
      

//    f_Color = vec4(v_TexCoords.xy, 0.0f, 1.0f);
        //f_Color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}