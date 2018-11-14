#version 330 core
out vec4 FragColor;
in vec3 ourPosition;
in vec2 texCoord;
in vec3 ourColor;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float mixAmount;
void main()
{
    FragColor = mix(texture(texture1, texCoord), texture(texture2, vec2(1 - texCoord.x, texCoord.y)), mixAmount) * vec4(ourColor, 1.0);
}