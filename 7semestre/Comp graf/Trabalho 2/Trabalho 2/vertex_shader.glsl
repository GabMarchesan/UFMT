#version 330 core
layout(location = 0) in vec3 position;   // Posição do vértice
layout(location = 1) in vec3 normal;     // Normal do vértice

uniform mat4 model;       // Matriz de modelo
uniform mat4 view;        // Matriz de visualização (câmera)
uniform mat4 projection;  // Matriz de projeção

out vec3 FragPos;         // Posição do fragmento no espaço do mundo
out vec3 Normal;          // Normal interpolada

void main()
{
    FragPos = vec3(model * vec4(position, 1.0)); // Calcula a posição do fragmento
    Normal = mat3(transpose(inverse(model))) * normal;  // Ajusta a normal

    gl_Position = projection * view * vec4(FragPos, 1.0);  // Transforma o vértice em coordenadas de tela
}
