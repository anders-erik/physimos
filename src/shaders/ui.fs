#version 330 core
out vec4 FragColor;

in vec3 colorOut;
uniform vec3 elementColor;

void main()
{
    FragColor = vec4(colorOut, 1.0f);
    // FragColor = vec4(elementColor, 1.0f);
}
