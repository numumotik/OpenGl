out vec4 color;

uniform struct PointLight {
int type;
vec4 position;
vec4 ambient;
vec4 diffuse;
vec4 specular;
vec3 attenuation;
vec3 spotDirection;
float spotCutoff;
float spotExponent;
} light;

uniform	sampler2D myTexture;

uniform struct Material {
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec4 emission;
	float shiness;
} material;

in Vertex{
vec2 texcoord;
vec3 normal;
vec3 lightDir;
vec3 viewDir;
float distance;
} Vert;

vec4 colorPoint()
{
    vec4 counted = vec4(1.0,1.0,1.0,1.0);
    
    vec3 normal = normalize(Vert.normal);
	vec3 lightDir = normalize(Vert.lightDir);
	vec3 viewDir = normalize(Vert.viewDir);

    float attenuation = 1.0 / (light.attenuation[0] + light.attenuation[1] * Vert.distance + light.attenuation[2] * Vert.distance*Vert.distance);
	counted = material.emission;
	counted += material.ambient*light.ambient*attenuation;
	float Ndot = max(dot(normal, lightDir), 0.0);
	counted += material.diffuse*light.diffuse*Ndot*attenuation;
	float RdotVpow = max(pow(dot(reflect(-lightDir, normal), viewDir), material.shiness), 0.0);
	counted += material.specular*light.specular*RdotVpow*attenuation;
    
    return counted;
}

vec4 colorDir()
{
    vec4 counted = vec4(1.0,1.0,1.0,1.0);
    vec3 normal = normalize(Vert.normal);
	vec3 lightDir = normalize(vec3(light.position));
	vec3 viewDir = normalize(Vert.viewDir);

	counted = material.emission;
	counted += material.ambient*light.ambient;
	float Ndot = max(dot(normal, lightDir), 0.0);
	counted += material.diffuse*light.diffuse*Ndot;
	float RdotVpow = max(pow(dot(reflect(-lightDir, normal), viewDir), material.shiness), 0.0);
	counted += material.specular*light.specular*RdotVpow;
    
    return counted;
}

vec4 colorSpot()
{
    vec4 counted = vec4(1.0,1.0,1.0,1.0);
     
    vec3 normal = normalize(Vert.normal);
	vec3 lightDir = normalize(Vert.lightDir);
	vec3 viewDir = normalize(Vert.viewDir);

    float spotEffect = dot(normalize(light.spotDirection), -lightDir);
    float spot = spotEffect > cos(light.spotCutoff)? 1.0 : 0.0;
    spotEffect = max(pow(spotEffect, light.spotExponent),0.0);
    float attenuation = spot * spotEffect / (light.attenuation[0] + light.attenuation[1] * Vert.distance + light.attenuation[2] * Vert.distance*Vert.distance);
	counted = material.emission;
	counted += material.ambient*light.ambient*attenuation;
	float Ndot = max(dot(normal, lightDir), 0.0);
	counted += material.diffuse*light.diffuse*Ndot*attenuation;
	float RdotVpow = max(pow(dot(reflect(-lightDir, normal), viewDir), material.shiness), 0.0);
	counted += material.specular*light.specular*RdotVpow*attenuation;
    
    return counted;
}
void main()
{
    if (light.type == 0)
        color = colorPoint();
    else 
        if (light.type == 1)
            color = colorSpot();
        else
            color = colorDir();
	color *= texture(myTexture, Vert.texcoord);
}