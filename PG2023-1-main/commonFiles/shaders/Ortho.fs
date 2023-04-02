//Código fonte do Fragment Shader (em GLSL)
#version 450
out vec4 color;
in vec4 finalColor;
void main()
{
    color = finalColor;
}