#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse1;
    //sampler2D diffuse2;
    //bool useMutiple;
    vec3 specular;    
    float shininess;
}; 
struct DirLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};
in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;
  
uniform vec3 viewPos;
uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLight;
uniform SpotLight spotLight;
uniform int turn;
uniform int no_texture;
uniform vec3 cubeColor;

vec3 dirLightFun(DirLight light,vec3 normal, vec3 viewDir);
vec3 pointLightFun(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 spotLightFun(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // phase 1: dir light
    vec3 result = dirLightFun(dirLight, norm, viewDir);
    // phase 2: point lights
    result += pointLightFun(pointLight, norm, FragPos, viewDir);    
    // phase 3: spot light
    //result += spotLightFun(spotLight, norm, FragPos, viewDir);    
    
    FragColor = vec4(result, 1.0);
	
} 

vec3 dirLightFun(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 multi;
    if(no_texture == 0){
        multi = vec3(texture(material.diffuse1, TexCoords));
    }else{
        multi = cubeColor;
    }
    
    vec3 ambient = light.ambient * multi;
    vec3 diffuse = light.diffuse * diff * multi;
    vec3 specular = light.specular * spec * multi;
    
    if(turn == 0) return (ambient + diffuse + specular);
    else return ambient;
}

// calculates the color when using a point light.
vec3 pointLightFun(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 0.5 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    vec3 multi;
    if(no_texture == 0){
        multi = vec3(texture(material.diffuse1, TexCoords));
    }else{
        multi = cubeColor;
    }
    
    
    vec3 ambient = light.ambient * multi;
    vec3 diffuse = light.diffuse * diff * multi;
    vec3 specular = light.specular * spec * material.specular;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    if(turn == 0) return (ambient + diffuse + specular);
    else return ambient;
}
vec3 spotLightFun(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 multi;
    if(no_texture == 0){
        multi = vec3(texture(material.diffuse1, TexCoords));
    }else{
        multi = cubeColor;
    }
    
    
    vec3 ambient = light.ambient * multi;
    vec3 diffuse = light.diffuse * diff * multi;
    vec3 specular = light.specular * spec * material.specular;
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    if(turn == 0) return (ambient + diffuse + specular);
    else return ambient;
}