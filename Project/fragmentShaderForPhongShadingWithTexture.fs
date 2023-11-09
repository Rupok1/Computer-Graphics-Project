#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D ambient;
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct PointLight {
    vec3 position;
    
    float k_c;  // attenuation factors
    float k_l;  // attenuation factors
    float k_q;  // attenuation factors
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
struct SpotLight {
    vec3 direction;
    vec3 position;
    float k_c;  // attenuation factors
    float k_l;  // attenuation factors
    float k_q;  // attenuation factors
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 emission;

};

struct DirectionLight {
    vec3 position;
    float k_c;  // attenuation factors
    float k_l;  // attenuation factors
    float k_q;  // attenuation factors
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
   

};


#define NR_POINT_LIGHTS 10
#define NR_SPOT_LIGHTS 4
#define NR_DIRECTION_LIGHTS 4

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLights[NR_SPOT_LIGHTS];
uniform DirectionLight directionLights[NR_DIRECTION_LIGHTS];

uniform Material material;

// function prototypes
vec3 CalcPointLight(Material material, PointLight light, vec3 N, vec3 fragPos, vec3 V);

vec3 CalcSpotLight(Material material, SpotLight light, vec3 N, vec3 fragPos, vec3 V)
{
    vec3 L = normalize(light.position - fragPos);
    vec3 R = reflect(-L, N);
    
   
   
    
    // attenuation
    float d = length(light.position - fragPos);
  //  float attenuation = 1.0 / (light.k_c + light.k_l * d + light.k_q * (d * d));
    
    float attenuation = 0;
    float theta = cos(30);
    vec3 V_l = -normalize(light.position-light.direction);
    vec3 V_o = normalize(fragPos-light.position);

    if(dot(V_l,V_o) >= theta){
       attenuation = pow(dot (V_l, V_o),64);
    }
    else
    {
        attenuation = 0;
    }

    vec3 ambient = vec3(texture(material.ambient, TexCoords)) * light.ambient;
    vec3 diffuse = vec3(texture(material.diffuse, TexCoords)) * max(dot(N, L), 0.0) * light.diffuse;
    vec3 specular = vec3(texture(material.specular, TexCoords)) * pow(max(dot(V, R), 0.0), material.shininess) * light.specular;

    
    
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    
    return (ambient + diffuse + specular);
}


vec3 CalcDirectionLight(Material material, DirectionLight light, vec3 N, vec3 fragPos, vec3 V)
{
    vec3 L = normalize(light.position - fragPos);
    vec3 R = reflect(-L, N);
    
   

    
    // attenuation
    float d = length(light.position - fragPos);
    //float attenuation = 1.0 / (light.k_c + light.k_l * d + light.k_q * (d * d));
    float attenuation = 0;

    if(d>0){
      attenuation =1;
    }
    else
    {
        attenuation = 1.0 / (light.k_c + light.k_l * d + light.k_q * (d * d));
    }
    
    vec3 ambient = vec3(texture(material.ambient, TexCoords)) * light.ambient;
    vec3 diffuse = vec3(texture(material.diffuse, TexCoords)) * max(dot(N, L), 0.0) * light.diffuse;
    vec3 specular = vec3(texture(material.specular, TexCoords)) * pow(max(dot(V, R), 0.0), material.shininess) * light.specular;
    
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    
    return (ambient + diffuse + specular);

}


void main()
{
    // properties
    vec3 N = normalize(Normal);
    vec3 V = normalize(viewPos - FragPos);
    
    vec3 result;
    // point lights
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
    {
    result += CalcPointLight(material, pointLights[i], N, FragPos, V);
   
    }
         result += CalcSpotLight(material, spotLights[0], N, FragPos, V);
    result += CalcDirectionLight(material, directionLights[0], N, FragPos, V);
      
    FragColor = vec4(result, 1.0);
}

// calculates the color when using a point light.
vec3 CalcPointLight(Material material, PointLight light, vec3 N, vec3 fragPos, vec3 V)
{
    vec3 L = normalize(light.position - fragPos);
    vec3 R = reflect(-L, N);
    
    // attenuation
    float d = length(light.position - fragPos);
    float attenuation = 1.0 / (light.k_c + light.k_l * d + light.k_q * (d * d));
    
    vec3 ambient = vec3(texture(material.ambient, TexCoords)) * light.ambient;
    vec3 diffuse = vec3(texture(material.diffuse, TexCoords)) * max(dot(N, L), 0.0) * light.diffuse;
    vec3 specular = vec3(texture(material.specular, TexCoords)) * pow(max(dot(V, R), 0.0), material.shininess) * light.specular;
    
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    
    return (ambient + diffuse + specular);
}
