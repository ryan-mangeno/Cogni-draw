#shader vertex
#version 330 core


layout(location = 0) in vec3 aPos;    
layout(location = 1) in vec3 aNormal;   
layout(location = 2) in vec2 aTexCoords; 

out vec3 FragPos;  
out vec3 Normal;   
out vec2 TexCoords; 

uniform mat4 u_MVP;  

void main()
{
    FragPos = aPos;
    Normal = aNormal; 
    TexCoords = aTexCoords;

    gl_Position = u_MVP * vec4(aPos, 1.0);
}

#shader fragment
#version 330 core


in vec3 FragPos;  
in vec3 Normal;    
in vec2 TexCoords; 

out vec4 FragColor;

uniform vec3 u_ViewPos; 
uniform sampler2D u_DiffuseTexture;
uniform sampler2D u_SpecularTexture; 



void main()
{
    vec3 lightPos = vec3(0.0f, 0.0f, 10.0f); 
    vec3 lightColor = vec3(1.0f, 1.0f, 1.0f); 

    vec3 lightDir = normalize(lightPos - FragPos);

    vec3 norm = normalize(Normal);

    float diff = max(dot(norm, lightDir), 0.0f);

    vec3 viewDir = normalize(u_ViewPos - FragPos); 
    vec3 reflectDir = reflect(-lightDir, norm);   

	const float shininess = 16.0f; // shininess factor for specular highlights

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

    vec3 diffuse = texture(u_DiffuseTexture, TexCoords).rgb; 
    vec3 specular = texture(u_SpecularTexture, TexCoords).rgb; 

    vec3 ambient = 0.1 * diffuse; // ambient light
    vec3 diffuseColor = diff * diffuse * lightColor;
    vec3 specularColor = spec * specular * lightColor; 

    vec3 finalColor = ambient + diffuseColor + specularColor;

    FragColor = vec4(finalColor, 1.0f); 
}