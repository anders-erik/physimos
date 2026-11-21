
#version 330 core

// uniform float color;
uniform float time_s;

out vec4 FragColor;

float color_frequency_multiplier = 2.0;

void main()
{
    float color_component = 0.5 * sin(time_s * color_frequency_multiplier);
    //  float color_component = time_s;

    //  FragColor = vec4(color, 0.0, 0.0, 1.0);
    //  FragColor = vec4(0.5, 0.0, 0.0, 1.0);
    //  FragColor = vec4(0.3, 0.0, 0.0, 1.0);
    FragColor = vec4(color_component, color_component, color_component, 1.0);
    //  FragColor = vec4(sin(color_component), 0.0, 0.0, 1.0);
    //  FragColor = vec4(sin(time_s), 0.0, 0.0, 1.0);
}