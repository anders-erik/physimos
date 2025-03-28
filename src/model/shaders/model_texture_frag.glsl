#version 330 core

uniform sampler2D myTexture; 

in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;
// in vec3 FragPos;

void main() {
    vec4 text_color = texture(myTexture, TexCoords);
    vec3 text_vec3 = vec3(text_color);

    // vec4 text_color = texture(myTexture, vec2(0.5, 0.5));
    // text_color += vec4(0.0, 0.0, 0.0, 1.0);
    // vec4 text_color = vec4(1.0, 1.0, 1.0, 1.0);
    

    // vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0));
    vec3 lightDir = normalize(vec3(0.0, 0.0, 1.0));

    float diff = max(dot(Normal, lightDir), 0.0);

    // vec3 color = vec3(0.2, 0.9, 0.2) * diff;
    vec3 diffuse = vec3(0.2, 0.2, 0.2) * diff;

    vec3 ambient = vec3(0.1, 0.1, 0.1);
    diffuse += ambient; // Make sure we get some ambient light

    // FragColor = vec4(color, 0.5);
    // FragColor = vec4(1.0, 1.0, 1.0, 0.1);

    // FragColor = text_color;
    // FragColor = vec4(text_color.x, text_color.y, 0.0, 1.0);

    vec3 color = vec3( ambient + diffuse ) * text_vec3;

    // FragColor = vec4( color , 1.0);

    FragColor = vec4( color , 0.7);

    // FragColor = vec4(1.0, 1.0, 1.0, 1.0);
    // FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
