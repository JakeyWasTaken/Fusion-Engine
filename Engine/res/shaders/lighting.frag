#version 330 core
struct Material {
    vec3 ambient;
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 

struct Light {
    vec3 position;
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float cutOff;
    float outerCutOff;
    float constant;
    float linear;
    float quadratic;
};

out vec4 FragColor;
  
uniform vec3 objectColor;
uniform vec3 viewPos;
uniform Material material;
uniform Light light; 

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

void main()
{
    vec3 lightDir = normalize(light.position - FragPos);
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon   = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0); 
    
    vec4 color = vec4(0.0f);

    if(theta > light.outerCutOff) 
    {       
      // do lighting calculations
      float distance = length(light.position - FragPos);
        float attenuation = 1.0 / (light.constant + light.linear * distance + 
    		        light.quadratic * (distance * distance));  
  	
        // diffuse 
       vec3 norm = normalize(Normal);
       float diff = max(dot(norm, lightDir), 0.0);
    
        // specular
       vec3 viewDir = normalize(viewPos - FragPos);
       vec3 reflectDir = reflect(-lightDir, norm);  
       float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        
       vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords)) * attenuation;
       vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords)) * attenuation;  
       vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords)) * attenuation;
       color = vec4(ambient + diffuse * intensity + specular * intensity, 1.0);   
    }
    else  // else, use ambient light so scene isn't completely dark outside the spotlight.
      color = vec4(light.ambient * vec3(texture(material.diffuse, TexCoords)), 1.0);

    FragColor = color;
}  