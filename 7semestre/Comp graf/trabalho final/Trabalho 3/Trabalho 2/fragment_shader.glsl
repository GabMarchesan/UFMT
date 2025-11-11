#version 330 core
in vec3 FragPos;   // Posição do fragmento
in vec3 Normal;    // Normal interpolada

uniform vec3 lightPos;    // Posição da luz
uniform vec3 viewPos;     // Posição da câmera
uniform vec3 lightColor;  // Cor da luz
uniform vec3 objectColor; // Cor do objeto

out vec4 color;   // Cor final do fragmento

void main()
{
    // Propriedades de iluminação
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    
    // Componente difusa
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // Componente especular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = spec * lightColor;
    
    // Combina os componentes
    vec3 result = (diffuse + specular) * objectColor;
    color = vec4(result, 1.0);
}
